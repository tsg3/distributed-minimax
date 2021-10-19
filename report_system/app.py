import json
import os
import tkinter as tk
from pathlib import Path

home = None

def open_report_as_dict():
    path = os.path.join(home, "report.json")
    data = None

    with open(path) as json_file:
        data = json.load(json_file)
    
    return data

if __name__ == "__main__":
    home = str(Path.home())

    report = open_report_as_dict()
    print(report)

    root = tk.Tk()
    root.title("Sistema de visualización")
    root.config(bg="#f1f1f1")
    root.resizable(width=False, height=False)

    root.mainloop()