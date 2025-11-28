from ctypes import CDLL, c_char_p, c_double
import customtkinter as ctk

def backspace():
    current_text = display.get()
    display.delete(0, "end")
    display.insert(0, current_text[:-1])

ctk.set_appearance_mode("light")
#ctk.set_default_color_theme("G:/c/stacks/theme.json")
lib = CDLL("G:/c/stacks/1.so")

lib.evaluate.argtypes = [c_char_p]
lib.evaluate.restype = c_double

app = ctk.CTk()
app.geometry("300x400")
app.title("Calculator")


display = ctk.CTkEntry(app, width = 300, height = 60, font = ("Arial", 24), justify = "right")
display.grid(row=0, column=0, columnspan=8, padx=10, pady=10)

buttons = [
    "7", "8", "9", "/", "backspace",
    "4", "5", "6", "*", "clear",
    "1", "2", "3", "-", "delete",
    "0", ".", "+", "(", ")",
    "[", "]", "{" , "}","=",
]

x = 1

def on_enter(event):
    k = display.get()
    z = lib.evaluate(k.encode())
    print(z)
    clear()
    display.insert("end", z)
    x = 0

def clear():
    display.delete(0, "end")

def throw(a):
    global x
    if a=="=":
        text = display.get()
        k = lib.evaluate(text.encode())
        print(k)
        clear()
        display.insert("end", k)
        x = 0
    else:
        if x:
            display.insert("end", a)
        else:
            clear()
            display.insert("end", a)
            x = 1

row = 1
col = 0

for btn_text in buttons:
    if btn_text == "clear":
        btn = ctk.CTkButton(app, text=btn_text, width=80, height=60, command=clear)
    elif btn_text == "backspace":
        btn = ctk.CTkButton(app, text=btn_text, width=100, height=60, command=backspace)
    else:
        btn = ctk.CTkButton(app, text=btn_text, width=60, height=60, command=lambda v=btn_text: throw(v))
    btn.grid(row=row, column=col, padx=5, pady=5, sticky="nsew")
    
    col += 1
    if col > 4:  # 5 buttons per row
        col = 0
        row += 1

for i in range(5):
    app.grid_columnconfigure(i, weight=1)
for i in range(7):
    app.grid_rowconfigure(i, weight=1)

display.bind("<Return>", on_enter)

app.mainloop()
            
