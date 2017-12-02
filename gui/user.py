# -*- coding: utf-8 -*-

from enum import Enum, unique

@unique
class UserState(Enum):

    Online = 0
    Offline = 1


class User(object):

    def __init__(self, conn, name, state, update):

        self.conn = conn
        self.name = name
        self.state = state
        self.update = update

    def update(self, seq):

        self.update += seq
