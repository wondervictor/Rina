# -*- coding: utf-8 -*-

import logging
import common


STYLE = {
        'fore':
        {
            'black': 30,
            'red': 31,
            'green': 32,
            'yellow': 33,
            'blue': 34,
            'purple': 35,
            'cyan': 36,
            'white': 37,
        },

        'back':
        {
            'black': 40,  
            'red': 41,  
            'green': 42,  
            'yellow': 43,  
            'blue': 44,  
            'purple': 45,  
            'cyan': 46,  
            'white': 47,  
        },

        'mode':
        {
            'mormal': 0,   
            'bold': 1,   
            'underline': 4,   
            'blink': 5,   
            'invert': 7,   
            'hide': 8,   
        },

        'default':
        {
            'end': 0,
        },
}


def stylize(msg, fore='', back='', mode=''):
    mode = '%s' % STYLE['mode'][mode] if STYLE['mode'].has_key(mode) else ''
    fore = '%s' % STYLE['fore'][fore] if STYLE['fore'].has_key(fore) else ''
    back = '%s' % STYLE['back'][back] if STYLE['back'].has_key(back) else ''
    style = ';'.join([s for s in [mode, fore, back] if s])
    style = '\033[%sm' % style if style else ''
    end = '\033[%sm' % STYLE['default']['end'] if style else ''
    return '%s%s%s' % (style, msg, end)


class Logger:

    def __init__(self, name):
        self.logger = logging.getLogger(name)
        self.logger.setLevel(logging.DEBUG)
        sh = logging.StreamHandler()
        self.logger.addHandler(sh)

    def get_timestr(self):
        timestamp = common.get_timestamp()
        date_str = common.time_to_str(timestamp)
        return date_str

    def warn(self, msg):
        title = "%s[WARN]" % self.get_timestr()
        msg = stylize(title, fore='yellow') + msg
        self.logger.warning(msg)

    def info(self, msg):

        title = "%s[INFO]" % self.get_timestr()
        msg = stylize(title, fore='green') + msg
        self.logger.warning(msg)

    def error(self, msg):

        title = "%s[ERROR]" % self.get_timestr()
        msg = stylize(title, fore='red') + msg
        self.logger.warning(msg)


