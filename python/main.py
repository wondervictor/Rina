# -*- coding: utf-8 -*-


"""
Content: Main View
Author: Vic Chan
Date: 2017/10/19
"""


from Tkinter import *
import tkMessageBox


class Application(Frame):

    def __init__(self, master=None):
        Frame.__init__(self, master)

        self.pack()
        self.create_widgets()

    def quit_button(self):
        print("I will quit")
        self.quit()

    def hello(self):

        name = self.nameInput.get() or 'world'
        tkMessageBox.showinfo('Message', 'Hello, %s' % name)

    def create_widgets(self):
        self.nameInput = Entry(self)
        self.nameInput.pack()
        self.hello_label = Label(self, text='Hello World')
        self.hello_label.pack()
        self.quit_button = Button(self, text='Quit', command=self.quit_button)
        self.quit_button.pack()

        self.hello_button = Button(self, text='Hello', command=self.hello)
        self.hello_button.pack()

        self.menu_button = Menubutton(
            self,

        )


app = Application()

app.master.title("Hello")

app.mainloop()
