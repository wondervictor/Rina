# -*- coding: utf-8 -*-
import Tkinter as tk
def main():
    root = tk.Tk()  # TkInter主进程
    text=tk.Text(root)  # Text控件
    text.tag_config('r', foreground='red')  # 设定Text文字格式
    text.tag_config('b', foreground='black')
    text.insert(0.0, 'Hello ', 'r')  # 插入Text文字
    text.insert(tk.END,'World !\n','b')
    text.see('0.0')
    text.mark_set('insert', '0.0')
    text.pack(side=tk.LEFT, fill=tk.BOTH) # 添加到pack布局
    tt=text.get("0.0", "end") # 获取所有文本
    scrl = tk.Scrollbar(root,orient=tk.VERTICAL) # 滚动条Scrollbar控件
    scrl.pack(side=tk.RIGHT, fill=tk.Y) #添加到pack布局
    text['yscrollcommand'] = scrl.set # Text绑定Scrollbar
    scrl['command'] = text.yview  # Scrollbar绑定Text
    def addLine():
        for i in range(int(var.get())):
            text.insert(tk.CURRENT,'Hello World !\n','b')
    def clear():
        text.delete(0.0, tk.END)  # 清空Text
    tk.Button(root, text='clear', command = clear).pack(fill = tk.X) # 添加按钮
    tk.Button(root, text='add', command = addLine).pack(fill = tk.X)
    var = tk.StringVar() # TkInter字符串
    var.set('8')
    tk.Entry(root, textvariable = var).pack(fill = tk.X) # 添加Entry
    tk.Label(root, text = 'lines').pack(fill = tk.X)  # 添加Label
    root.mainloop()
if __name__=="__main__":
    main()
