# -*- coding: utf-8 -*-

"""
Login

"""

import Tkinter as tk
from Tkinter import *


class Login(Frame):

    def __init__(self, master=None):
        Frame.__init__(self, master, width=200, height=200)
        self.config(width=200, height=200)

        self.pack()
        self.user_label = Label(self, text="User:")
        self.user_label.grid(row=0, column=0, sticky=W)
        self.user_entry = Entry(self)
        self.user_entry.grid(row=0, column=1, sticky=W)

        self.server_ip = Label(self, text="IP:")
        self.server_ip.grid(row=1, column=0, sticky=W)

        self.server_entry = Entry(self)
        self.server_entry.grid(row=1, column=1, sticky=W)

        self.login_button = Button(self, text="Login", command=self.login, height=5, width=20)
        self.login_button.grid(row=2, column=0, sticky=W)

        self.cancel_button = Button(self, text="Cancel", command=self.cancel)
        self.cancel_button.grid(row=3, column=0, sticky=W)

    def login(self):
        print("Login")

    def cancel(self):
        print("Cancel")


def center_window(root, width, height):
    screenwidth = root.winfo_screenwidth()
    screenheight = root.winfo_screenheight()
    size = '%dx%d+%d+%d' % (width, height, (screenwidth - width)/2, (screenheight - height)/2)
    print(size)
    root.geometry(size)


def test():

    root = Tk()
    root.wm_attributes('-topmost',1)
    center_window(root, 300, 200)
    app = Login(root)
    root.mainloop()

test()

