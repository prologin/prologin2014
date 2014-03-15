#! /usr/bin/env python2

def options(opt):
    pass

def configure(cfg):
    pass

def build(bld):
    bld.shlib(
        source = '''
            src/game-state.cc
            src/map.cc
            src/rules.cc
            src/entry.cc
            src/api.cc
            src/interface.cc
        ''',
        target = 'prologin2014',
        use = ['stechec2'],
        defines = ['MODULE_COLOR=ANSI_COL_PURPLE',
            'MODULE_NAME="prologin2014"'],
        lib = ['dl'],
    )

    for test in []:
        bld.program(
            features = 'gtest',
            source = 'src/tests/test-%s.cc' % test,
            target = 'prologin2014-test-%s' % test,
            use = ['prologin2014', 'stechec2-utils'],
            includes = ['.'],
            defines = ['MODULE_COLOR=ANSI_COL_PURPLE',
                'MODULE_NAME="prologin2014"'],
        )

    bld.install_files('${PREFIX}/share/stechec2/prologin2014', [
        'prologin2014.yml',
    ])

