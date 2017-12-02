# -*- coding: utf-8 -*-

import time
import datetime


def get_timestamp():

    return int(time.time())


def time_to_str(timestamp):

    return datetime.datetime.\
        fromtimestamp(timestamp).strftime("%Y-%m-%d %H:%M:%S")
