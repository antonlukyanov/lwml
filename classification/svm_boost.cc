#include "svm_boost.h"

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

namespace lwml {

svm::svm( const keypoint_list_lvset& kpl, bool train_immediately )
{
  _need_learn = true;

  // Move training set to svm_problem

  int len = kpl.len();
  _dim = kpl.dim();

  svm_node **x = new svm_node*[len]; // List of points in training set
  for( int i = 0; i < len; ++i ){
    x[i] = new svm_node[_dim + 1]; // Point (with index i) of training set
  }
  double *y = new double[len]; // Classes of training set(integers for classification, starting with one)

  for( int i = 0; i < len; i++ ){
    y[i] = kpl.loading(i);
    for( int j = 0; j < _dim; j++ ){
      x[i][j].value = kpl.get(i, j);
      x[i][j].index = j + 1;
    }
    x[i][_dim].index = -1;
  }

  // Move training set to svm_problem
  _svm_problem = new svm_problem;
  _svm_problem->l = len;
  _svm_problem->y = y;
  _svm_problem->x = x;

  init_scaling_params();
  scale_svm_problem();

  _params = get_default_params(_dim);

  // Check parameters for validity
  const char* str = svm_check_parameter(_svm_problem, _params);
  if( str == NULL )
    printf("Params is OK\n");
  else
    printf("%s\n", str);

  // Если необходимо, обучемся сразу
  if( train_immediately )
    start_train();
}

void svm::start_train()
{
  if( _need_learn ){
    _model = svm_train(_svm_problem, _params); // Train svm. Model contains a trained classifier.
    _need_learn = false;
  }else{
    fail_assert("Second call of start_train");
  }
}
svm::svm( const char* file_name_prefix )
{
  // Generate model without teaching
  _need_learn = false;
  _svm_problem = NULL;
  _params = NULL;
  zzz("Reading model for SVM");
  _model = svm_load_model(strng::form("%s_model.txt", file_name_prefix).ascstr());
  if( _model == NULL )
    fail_assert("Error: can't load %s");
  else{
    zzz("Reading model for SVM success");
    int idx = 0;
    int count = 0;
    for( int i = 0; idx != -1; i++ ){
      idx = _model->SV[0][i].index;
      count++;
    }
    _dim = count - 1;
  }
  zzz("Reading scaling params");
  restore_scaling_params(strng::form("%s_range.txt", file_name_prefix).ascstr());
}

svm::svm( referer<luaconf> cnf, const char* root )
{
  // Generate model without teaching
  _need_learn = false;
  _svm_problem = NULL;

  // *** dim ***

  _dim = cnf->get_int("%s.svm.dim", root);

  // *** svm_parameter ***

  _params = new svm_parameter;
  _params->svm_type = cnf->get_int("%s.svm.param.svm_type", root);
  _params->kernel_type = cnf->get_int("%s.svm.param.kernel_type", root);
  _params->degree = cnf->get_int("%s.svm.param.degree", root);
  _params->gamma = cnf->get_real("%s.svm.param.gamma", root);
  _params->coef0 = cnf->get_real("%s.svm.param.coef0", root);

  _params->cache_size = cnf->get_real("%s.svm.param.cache_size", root);
  _params->eps = cnf->get_real("%s.svm.param.eps", root);
  _params->C = cnf->get_real("%s.svm.param.C", root);
  _params->nr_weight = cnf->get_int("%s.svm.param.nr_weight", root);
  if( _params->nr_weight == 0 ){
    _params->weight_label = NULL;
    _params->weight = NULL;
  }else{
    _params->weight_label = Malloc(int, _params->nr_weight);
    _params->weight = Malloc(double, _params->nr_weight);

    for( int i = 0; i < _params->nr_weight; ++i ){
      _params->weight_label[i] = cnf->get_int("%s.svm.param.weight_label[%d]", root, i);
      _params->weight[i] = cnf->get_int("%s.svm.param.weight[%d]", root, i);
    }
  }
  _params->nu = cnf->get_real("%s.svm.param.nu", root);
  _params->p = cnf->get_real("%s.svm.param.p", root);
  _params->shrinking = cnf->get_int("%s.svm.param.shrinking", root);
  _params->probability = cnf->get_int("%s.svm.param.probability", root);

  // *** model ***

  _model = Malloc(svm_model, 1);
  _model->param = *_params;
  _model->nr_class = cnf->get_int("%s.svm.nr_class", root);
  int k = _model->nr_class;
  _model->l = cnf->get_int("%s.svm.l", root);
  int nSV = _model->l;
  _model->SV = Malloc(svm_node *, nSV);
  for( int i = 0; i < nSV; ++i ){
    _model->SV[i] = Malloc(svm_node, _dim + 1);
    for( int j = 0; j <= _dim; ++j ){
      _model->SV[i][j].index = cnf->get_int("%s.svm.SV[%d][%d].index", root, i, j);
      _model->SV[i][j].value = cnf->get_real("%s.svm.SV[%d][%d].value", root, i, j);
    }
  }
  _model->sv_coef = Malloc(double *, k - 1);
  for( int i = 0; i < k - 1; i++ ){
    _model->sv_coef[i] = Malloc(double, nSV);
    for( int j = 0; j < nSV; ++j ){
      _model->sv_coef[i][j] = cnf->get_real("%s.svm.sv_coef[%d][%d]", root, i, j);
    }
  }
  int n = _model->nr_class * (_model->nr_class - 1) / 2;
  _model->rho = Malloc(double, n);
  for( int i = 0; i < n; ++i ){
    _model->rho[i] = cnf->get_real("%s.svm.rho[%d]", root, i);
  }

  _model->probA = Malloc(double, n);
  _model->probB = Malloc(double, n);
  for( int i = 0; i < n; ++i ){
    _model->probA[i] = cnf->get_real("%s.svm.probA[%d]", root, i);
    _model->probB[i] = cnf->get_real("%s.svm.probB[%d]", root, i);
  }

  _model->sv_indices = Malloc(int, nSV);
  for( int i = 0; i < nSV; ++i ){
    _model->sv_indices[i] = cnf->get_int("%s.svm.sv_indices[%d]", root, i);
  }

  _model->label = Malloc(int, k);
  for( int i = 0; i < k; ++i ){
    _model->label[i] = cnf->get_int("%s.svm.label[%d]", root, i);
  }

  _model->nSV = Malloc(int, k);
  for( int i = 0; i < k; ++i ){
    _model->nSV[i] = cnf->get_int("%s.svm.nSV[%d]", root, i);
  }

  _model->free_sv = cnf->get_int("%s.svm.free_sv", root);

  // *** scaling_params ***

  _lower = cnf->get_real("%s.svm.scaling_params.lower", root);
  _upper = cnf->get_real("%s.svm.scaling_params.upper", root);
  _scaling_params = new scaling_params[_dim];
  for( int i = 0; i < dim(); ++i ){
    _scaling_params[i].min_value = cnf->get_real("%s.svm.scaling_params.sp_table[%d].min_value",
        root, i);
    _scaling_params[i].max_value = cnf->get_real("%s.svm.scaling_params.sp_table[%d].max_value",
        root, i);
  }
}

void svm::init_scaling_params()
{
  // Все значения масштабируются от -1 до 1
  _lower = -1;
  _upper = 1;
  _scaling_params = new scaling_params[_dim];

  // Находим минимальное и максимальное значения для каждого признака
  for( int j = 0; j < _dim; j++ ){
    _scaling_params[j].max_value = REAL_MIN;
    _scaling_params[j].min_value = REAL_MAX;
  }
  for( int j = 0; j < _dim; j++ ){
    for( int i = 0; i < _svm_problem->l; i++ ){
      if( _svm_problem->x[i][j].value < _scaling_params[j].min_value )
        _scaling_params[j].min_value = _svm_problem->x[i][j].value;
      else if( _svm_problem->x[i][j].value > _scaling_params[j].max_value )
        _scaling_params[j].max_value = _svm_problem->x[i][j].value;
    }
  }
}
void svm::scale_vector( vector& vec ) const
{
  for( int i = 0; i < _dim; i++ ){
    vec[i] = scale_feature(vec[i], i);
  }
}

real svm::scale_feature( real feature_value, int ind ) const
{
  return feature_value = -1
      + (1 - (-1)) * (feature_value - _scaling_params[ind].min_value)
      / (_scaling_params[ind].max_value - _scaling_params[ind].min_value);
}
void svm::scale_svm_problem()
{
  for( int i = 0; i < _svm_problem->l; i++ ){
    for( int j = 0; j < _dim; j++ ){
      _svm_problem->x[i][j].value = scale_feature(_svm_problem->x[i][j].value, j);
    }
  }
}
void svm::free_scaling_params()
{
  delete[] _scaling_params;
}

int svm::dim() const
{
  return _dim;
}
int svm::class_num() const
{
  return _model->nr_class;
}
int svm::step_num() const
{
  // TODO
  return 0;
}

int svm::classify( const vector& vec, int num ) const
{
  zzz("SVM boosting classifier, vec_len = %d, _dim = %d", vec.len(), _dim);
  vector scaled_vec(vec);
  scale_vector(scaled_vec);

  if( vec.len() != _dim )
    return -1;
  svm_node *test_node = new svm_node[_dim + 1]; // Test point
  for( int i = 0; i < _dim; ++i ){
    test_node[i].value = scaled_vec.at(i);
    test_node[i].index = i + 1;
  }
  test_node[_dim].index = -1;
  int res_class = svm_predict(_model, test_node); // Классификация точки
  delete[] test_node;
  zzz("Point in %d class", res_class);
  return res_class;
}
void svm::get_feature_quality( vector& q, int num, int m_idx ) const
{
}

real svm::get_confidence( const vector& vec, int num ) const
{
  int nr_class = _model->nr_class;
  if( svm_check_probability_model(_model) ){
    vector scaled_vec(vec);
    scale_vector(scaled_vec);

    if( vec.len() != _dim )
      return -1;
    svm_node *test_node = new svm_node[_dim + 1]; // Test point
    for( int i = 0; i < _dim; ++i ){
      test_node[i].value = scaled_vec.at(i);
      test_node[i].index = i + 1;
    }
    test_node[_dim].index = -1;
    double *prob_estimates = new double[nr_class]; // Вероятности отнесения точки к каждому из классов(в сумме = 1)

    // Получаем вероятность отнесения точки к каждому из классов
    svm_predict_probability(_model, test_node, prob_estimates);

    // Находим степень уверенности классифкации (максимальное значение среди оценок вероятностей prob_estimates)
    int prob_max_idx = 0;
    for( int i = 1; i < nr_class; i++ )
      if( prob_estimates[i] > prob_estimates[prob_max_idx] )
        prob_max_idx = i;
    double prob = prob_estimates[prob_max_idx];

    if( nr_class == 2 )
      zzz("Probability info: %f %f", prob_estimates[0], prob_estimates[1]);
    else if( nr_class == 3 )
      zzz("Probability info: %f %f %f", prob_estimates[0], prob_estimates[1], prob_estimates[2]);

    delete[] test_node;
    delete[] prob_estimates;
    return prob;
  }else{
    zzz("Classification confidence is not support for this model :(");
    return 0;
  }
}

real svm::get_confidence_for_one_class( const vector& vec, int num, int class_idx) const
{
  int nr_class = _model->nr_class;
  if( svm_check_probability_model(_model) ){
    vector scaled_vec(vec);
    scale_vector(scaled_vec);

    if( vec.len() != _dim )
      return -1;
    svm_node *test_node = new svm_node[_dim + 1]; // Test point
    for( int i = 0; i < _dim; ++i ){
      test_node[i].value = scaled_vec.at(i);
      test_node[i].index = i + 1;
    }
    test_node[_dim].index = -1;
    double *prob_estimates = new double[nr_class]; // Вероятности отнесения точки к каждому из классов(в сумме = 1)

    // Получаем вероятность отнесения точки к каждому из классов
    svm_predict_probability(_model, test_node, prob_estimates);

    if (class_idx >= nr_class || class_idx < 0)
      fail_assert("Class index is out of range");

    double prob = prob_estimates[class_idx];

    delete[] test_node;
    delete[] prob_estimates;
    return prob;
  }else{
    zzz("Classification confidence is not support for this model :(");
    return 0;
  }
}

void svm::save_result( referer<luaconf> res, const char* root ) const
{
  // основная таблица с параметрами обученного классификатора
  res->exec("%s.svm = {}", root);

  // *** svm_parameter ***
  res->exec("%s.svm.param = {}", root);
  res->exec("%s.svm.param.svm_type = %d", root, _model->param.svm_type);
  res->exec("%s.svm.param.kernel_type = %d", root, _model->param.kernel_type);
  res->exec("%s.svm.param.degree = %d", root, _model->param.degree);
  res->exec("%s.svm.param.gamma = %f", root, _model->param.gamma);
  res->exec("%s.svm.param.coef0 = %f", root, _model->param.coef0);
  res->exec("%s.svm.param.cache_size = %f", root, _model->param.cache_size);
  res->exec("%s.svm.param.eps = %f", root, _model->param.eps);
  res->exec("%s.svm.param.C = %f", root, _model->param.C);
  res->exec("%s.svm.param.nr_weight = %d", root, _model->param.nr_weight);
  res->exec("%s.svm.param.weight_label = {}", root);
  res->exec("%s.svm.param.weight = {}", root);
  for( int i = 0; i < _model->param.nr_weight; ++i ){
    res->exec("%s.svm.param.weight_label[%d] = %d", root, i, _model->param.weight_label[i]);
    res->exec("%s.svm.param.weight[%d] = %f", root, i, _model->param.weight[i]);
  }
  res->exec("%s.svm.param.nu = %f", root, _model->param.nu);
  res->exec("%s.svm.param.p = %f", root, _model->param.p);
  res->exec("%s.svm.param.shrinking = %d", root, _model->param.shrinking);
  res->exec("%s.svm.param.probability = %d", root, _model->param.probability);

  // *** model ***

  // количество классов
  res->exec("%s.svm.nr_class = %d", root, _model->nr_class);
  int k = _model->nr_class;
  // total SV
  res->exec("%s.svm.l = %d", root, _model->l);

  res->exec("%s.svm.SV = {}", root);
  for( int i = 0; i < _model->l; i++ ){
    res->exec("%s.svm.SV[%d] = {}", root, i);
    for( int j = 0; j <= _dim; ++j ){
      res->exec("%s.svm.SV[%d][%d] = {}", root, i, j);
      res->exec("%s.svm.SV[%d][%d].index = %d", root, i, j, _model->SV[i][j].index);
      res->exec("%s.svm.SV[%d][%d].value = %.8g", root, i, j, _model->SV[i][j].value);
    }
  }

  res->exec("%s.svm.sv_coef = {}", root);
  for( int i = 0; i < k - 1; i++ ){
    res->exec("%s.svm.sv_coef[%d] = {}", root, i);
    for( int j = 0; j < _model->l; j++ ){
      //res->exec("%s.svm.sv_coef[%d][%d] = {}", root, i, j);
      res->exec("%s.svm.sv_coef[%d][%d] = %.16g", root, i, j, _model->sv_coef[i][j]);
    }
  }

  res->exec("%s.svm.rho = {}", root);
  for( int i = 0; i < (k * (k - 1)) / 2; ++i ){
    res->exec("%s.svm.rho[%d] = %f", root, i, _model->rho[i]);
  }

  res->exec("%s.svm.probA = {}", root);
  res->exec("%s.svm.probB = {}", root);
  for( int i = 0; i < (k * (k - 1)) / 2; ++i ){
    res->exec("%s.svm.probA[%d] = %f", root, i, _model->probA[i]);
    res->exec("%s.svm.probB[%d] = %f", root, i, _model->probB[i]);
  }

  res->exec("%s.svm.sv_indices = {}", root);
  for( int i = 0; i < _model->l; ++i ){
    res->exec("%s.svm.sv_indices[%d] = %d", root, i, _model->sv_indices[i]);
  }

  res->exec("%s.svm.label = {}", root);
  for( int i = 0; i < k; ++i ){
    res->exec("%s.svm.label[%d] = %d", root, i, _model->label[i]);
  }

  res->exec("%s.svm.nSV = {}", root);
  for( int i = 0; i < k; ++i ){
    res->exec("%s.svm.nSV[%d] = %d", root, i, _model->nSV[i]);
  }

  res->exec("%s.svm.free_sv = %d", root, _model->free_sv);

  // *** dim ***

  res->exec("%s.svm.dim = %d", root, _dim);

  // *** scaling_params ***

  res->exec("%s.svm.scaling_params = {}", root);
  res->exec("%s.svm.scaling_params.lower = %f", root, _lower);
  res->exec("%s.svm.scaling_params.upper = %f", root, _upper);

  res->exec("%s.svm.scaling_params.sp_table = {}", root);
  for( int i = 0; i < dim(); ++i ){
    res->exec("%s.svm.scaling_params.sp_table[%d] = {}", root, i);
    res->exec("%s.svm.scaling_params.sp_table[%d].min_value = %f", root, i,
        _scaling_params[i].min_value);
    res->exec("%s.svm.scaling_params.sp_table[%d].max_value = %f", root, i,
        _scaling_params[i].max_value);
  }
}

void svm::save_result_file( const char *file_name_prefix ) const
{
  svm_save_model(strng::form("%s_model.txt", file_name_prefix).ascstr(), _model);
  save_scaling_params(strng::form("%s_range.txt", file_name_prefix).ascstr());
}

void svm::restore_scaling_params( const char* fname )
{
  _scaling_params = new scaling_params[_dim];
  FILE* fp_restore = fopen(fname, "rt");
  if( fp_restore == NULL )
    zzz("ERROR: can't find %s", fname);
  if( fgetc(fp_restore) == 'x' ){
    if( fscanf(fp_restore, "%lf %lf\n", &_lower, &_upper) != 2 ){
      zzz("ERROR: failed to read scaling parameters\n");
      fclose(fp_restore);
      return;
    }
    zzz("lower = %f, upper = %f", _lower, _upper);
    int idx;
    double fmin, fmax;
    while( fscanf(fp_restore, "%d %lf %lf\n", &idx, &fmin, &fmax) == 3 ){
      zzz("Read scale parameter %d %f %f", idx, fmin, fmax);
      _scaling_params[idx - 1].min_value = fmin;
      _scaling_params[idx - 1].max_value = fmax;
    }

  }
  fclose(fp_restore);
}

svm::~svm()
{
  if( _params != NULL ){
    svm_destroy_param(_params);
    delete _params;
  }
  svm_free_and_destroy_model(&_model);
  // Free svm problem.
  if( _svm_problem != NULL ){
    for( int i = 0; i < _svm_problem->l; ++i ){
      delete[] _svm_problem->x[i];
    }
    delete[] _svm_problem->x;
    delete[] _svm_problem->y;
    delete _svm_problem;
  }
  delete[] _scaling_params;
}

svm_parameter* svm::get_default_params( int num_features )
{
  svm_parameter *param = new svm_parameter;
  // default values
  param->svm_type = C_SVC;
  param->kernel_type = RBF;
  param->degree = 3;
  param->gamma = 1.0 / num_features;  // 1/num_features
  param->coef0 = 0;
  param->nu = 0.5;
  param->cache_size = 300;
  param->C = 1;
  param->eps = 1e-3;
  param->p = 0.1;
  param->shrinking = 1;
  param->probability = 0;
  param->nr_weight = 0;
  param->weight_label = NULL;
  param->weight = NULL;

  param->probability = 1;
  param->C = 32;
  param->gamma = 0.5;

  return param;
}

void svm::save_scaling_params( const char* fname ) const
{
  FILE* fp_save = fopen(fname, "wt");
  fprintf(fp_save, "x\n");
  fprintf(fp_save, "%.16g %.16g\n", _lower, _upper);
  for( int i = 0; i < _dim; i++ )
    fprintf(fp_save, "%d %.16g %.16g\n", i + 1, _scaling_params[i].min_value,
        _scaling_params[i].max_value);
  fclose(fp_save);
}

void svm::set_svm_type_c_svc( double C, int nr_weight, int *weight_label, double* weight )
{
  _params->kernel_type = C_SVC;
  _params->C = C;
  _params->nr_weight = nr_weight;
  _params->weight_label = weight_label;
  _params->weight = weight;
}

void svm::set_svm_type_nu_svc( double nu )
{
  _params->kernel_type = NU_SVC;
  _params->nu = nu;
}

void svm::set_svm_type_one_class( double nu )
{
  _params->kernel_type = ONE_CLASS;
  _params->nu = nu;
}

void svm::set_svm_type_epsilon_svr( double C, double p )
{
  _params->kernel_type = EPSILON_SVR;
  _params->C = C;
  _params->p = p;
}

void svm::set_svm_type_nu_svr( double C, double nu )
{
  _params->kernel_type = EPSILON_SVR;
  _params->C = C;
  _params->nu = nu;
}

void svm::set_kernel_linear()
{
  _params->kernel_type = LINEAR;
}

void svm::set_kernel_poly( int degree, double gamma, double coef0 )
{
  _params->kernel_type = POLY;
  _params->degree = degree;
  _params->gamma = gamma;
  _params->coef0 = coef0;
}

void svm::set_kernel_rbf( double gamma )
{
  _params->kernel_type = RBF;
  _params->gamma = gamma;
}

void svm::set_kernel_sigmoid( double gamma, double coef0 )
{
  _params->kernel_type = SIGMOID;
  _params->gamma = gamma;
  _params->coef0 = coef0;
}

void svm::set_eps( double eps )
{
  _params->eps = eps;
}

void svm::set_cache_size( double cache_size )
{
  _params->cache_size = cache_size;
}

void svm::set_shrinking( int shrinking )
{
  _params->shrinking = shrinking;
}

void svm::set_probability( int probabiliry )
{
  _params->probability = probabiliry;
}

}
