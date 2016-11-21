import os


def find_lwml_sources(search_path, ext, excludes=[]):
    sources = []
    paths = []
    path = search_path
    while path:
        if path:
            for entry in os.scandir(path):
                fname = entry.name
                if not fname.startswith('.'):
                    excluded = False
                    for ex in excludes:
                        if ex in entry.path:
                            excluded = True
                            break
                    if entry.is_file() and fname.endswith(ext) and not excluded:
                        sources.append(entry.path.replace(search_path, '').strip('/'))
                    elif entry.is_dir():
                        paths.append(entry.path)
        try:
            path = paths.pop()
        except IndexError:
            path = False

    return sources


def options(opt):
    pass

def configure(conf):
    pass


def build(bld):
    bld.recurse('libs/llogsrv')

    excludes = ['libs/llogsrv', 'ihrng', 'svm_classifier', 'crc32']

    sources = find_lwml_sources(
        bld.path.abspath(),
        '.cc',
        excludes
    )

    bld.shlib(
        source=sources,
        target='lwml',
        name='lwml_static',
        install_path='lib'
    )

    bld.stlib(
        source=sources,
        target='lwml',
        name='lwml_dynamic',
        install_path='lib'
    )

    bld.install_files(
        '${DESTDIR}/include/lwml',
        find_lwml_sources(
            bld.path.abspath(),
            '.h',
            excludes
        ),
        name='lwml_headers',
        relative_trick=True
    )
