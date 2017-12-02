# -*- coding: utf-8 -*-

import logging
import common


STYLE = {
        'fore':
        {
            'black'    : 30,   #  黑色
            'red'      : 31,   #  红色
            'green'    : 32,   #  绿色
            'yellow'   : 33,   #  黄色
            'blue'     : 34,   #  蓝色
            'purple'   : 35,   #  紫红色
            'cyan'     : 36,   #  青蓝色
            'white'    : 37,   #  白色
        },

        'back' :
        {
            'black'     : 40,  #  黑色
            'red'       : 41,  #  红色
            'green'     : 42,  #  绿色
            'yellow'    : 43,  #  黄色
            'blue'      : 44,  #  蓝色
            'purple'    : 45,  #  紫红色
            'cyan'      : 46,  #  青蓝色
            'white'     : 47,  #  白色
        },

        'mode' :
        {
            'mormal'    : 0,   #  终端默认设置
            'bold'      : 1,   #  高亮显示
            'underline' : 4,   #  使用下划线
            'blink'     : 5,   #  闪烁
            'invert'    : 7,   #  反白显示
            'hide'      : 8,   #  不可见
        },

        'default' :
        {
            'end' : 0,
        },
}

def stylize(msg, fore='', back='', mode=''):
    mode  = '%s' % STYLE['mode'][mode] if STYLE['mode'].has_key(mode) else ''
    fore  = '%s' % STYLE['fore'][fore] if STYLE['fore'].has_key(fore) else ''
    back  = '%s' % STYLE['back'][back] if STYLE['back'].has_key(back) else ''
    style = ';'.join([s for s in [mode, fore, back] if s])
    style = '\033[%sm' % style if style else ''
    end   = '\033[%sm' % STYLE['default']['end'] if style else ''
    return '%s%s%s' % (style, msg, end)


class Logger:

    def __init__(self, name):
        self.logger = logging.getLogger(name)
        self.logger.setLevel(logging.DEBUG)
        sh = logging.StreamHandler()
        self.logger.addHandler(sh)

    def warn(self, msg):
        timestamp = common.get_timestamp()
        date_str = common.time_to_str(timestamp)
        title = "%s[WARN]" % date_str
        msg = stylize(title, fore='yellow') + msg
        self.logger.warning(msg)

    def info(self, msg):
        timestamp = common.get_timestamp()
        date_str = common.time_to_str(timestamp)
        title = "%s[INFO]" % date_str

        msg = stylize(title, fore='green') + msg
        self.logger.warning(msg)

    def error(self, msg):
        timestamp = common.get_timestamp()
        date_str = common.time_to_str(timestamp)
        title = "%s[ERROR]" % date_str

        msg = stylize(title, fore='red') + msg
        self.logger.warning(msg)


