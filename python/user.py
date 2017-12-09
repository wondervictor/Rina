# -*- coding: utf-8 -*-

from enum import Enum

class UserState(Enum):

    Online = 0
    Offline = 1


class User(object):

    def __init__(self, conn, name, state, update):

        self.conn = conn
        self.name = name
        self.state = state
        self.update = update

    def update_seq(self, seq):

        self.update += seq
