import json
import tkinter as tk

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
from numpy.core.fromnumeric import size

lbl_real_time_value = None
lbl_cpu_time_value = None
lbl_error_found_value = None
lbl_error = None

fig_cpu = None
fig_ram = None
canvas_cpu = None
canvas_ram = None

def get_data(measures_list):
    data = [0,]
    times = [0,]

    for measure in measures_list:
        data.append(measure["porcentaje"])
        times.append(measure["tiempo"])

    return (times, data)

def set_figures(report):
    fig_cpu.clear()
    data_cpu = get_data(report["nodos"]["nodo_1"]["cpu"])
    plt = fig_cpu.add_subplot()
    plt.plot(data_cpu[0], data_cpu[1])
    plt.grid(True)
    plt.set_title("Consumo de CPU en el tiempo del Nodo 1")
    plt.set_ylabel("Consumo de CPU (%)")
    plt.set_xlabel("Tiempo (s)")
    fig_cpu.tight_layout()

    fig_ram.clear()
    data_ram = get_data(report["nodos"]["nodo_1"]["ram"])
    plt = fig_ram.add_subplot()
    plt.plot(data_ram[0], data_ram[1])
    plt.grid(True)
    plt.set_title("Uso de RAM en el tiempo del Nodo 1")
    plt.set_ylabel("Uso de RAM (kB)")
    plt.set_xlabel("Tiempo (s)")
    fig_ram.tight_layout()

    canvas_cpu.draw()
    canvas_ram.draw()

def set_current_report(report):
    lbl_real_time_value.configure(text=f"{report['tiempo_real']} s")
    lbl_cpu_time_value.configure(text=f"{report['tiempo_cpu']} s")

    error_found_str = None
    color = None
    if report['error_encontrado']:
        error_found_str = "Error"
        color = "#FF0000"
    
    else:
        error_found_str = "Exitosa"
        color = "#00FF00"

    lbl_error_found_value.configure(text=error_found_str, fg=color)
    lbl_error.configure(text=f"{report['error']}")

    set_figures(report)

def browse_report():
    filename = tk.filedialog.askopenfilename(
        initialdir = "~/",
        title = "Selecciona el reporte",
        filetypes = (("Archivos JSON",
                        "*.json*"),
                    ("Todos los archivos",
                        "*.*"))
    )

    if filename == "" or filename == ():
        return

    try:
        return set_current_report(open_report_as_dict(filename))
    except Exception as e:
        return tk.messagebox.showerror(
            title="Error", 
            message=f"El archivo '{filename}' no es un archivo válido!\nIntentelo de nuevo"
        )

def open_report_as_dict(path):
    with open(path) as json_file:
        data = json.load(json_file)
    
    return data

if __name__ == "__main__":
    # Root container

    root = tk.Tk()
    root.title("Sistema de visualización")
    root.config(bg="#f1f1f1")

    # Main containers

    info_canvas = tk.Canvas(root, bg="#f1f1f1")
    info_canvas.grid_columnconfigure(2, weight=1)
    scroll_canvas = tk.Canvas(root, bg="#f1f1f1")
    scroll_v = tk.Scrollbar(root, command=scroll_canvas.yview)
    scroll_h = tk.Scrollbar(root, command=scroll_canvas.xview, orient="horizontal")

    # Scrollable Frame

    right_frame = tk.Frame(scroll_canvas, bg="#f1f1f1")

    info_canvas.pack(side="left", fill="y", expand=False)
    scroll_v.pack(side="right", fill="both", expand=False)
    scroll_h.pack(side="bottom", fill="x", expand=False)
    scroll_canvas.pack(side="right", fill="both", expand=True)

    right_frame.pack(fill="both", expand=True)

    # Scroll configuration

    right_frame.bind(
        "<Configure>",
        lambda e: scroll_canvas.configure(
            scrollregion=scroll_canvas.bbox("all")
        )
    )

    scroll_canvas.create_window((0, 0), window=right_frame)

    scroll_canvas.configure(yscrollcommand=scroll_v.set)

    scroll_canvas.configure(xscrollcommand=scroll_h.set)

    # 'info_canvas' widgets

    button_explore = tk.Button(info_canvas, text="Cargar reporte", command=browse_report)
    button_explore.grid(row=0, column=0, sticky=tk.E + tk.N + tk.S + tk.W)

    lbl_real_time = tk.Label(info_canvas, text="Tiempo Real:", bg="#f1f1f1")
    lbl_real_time.grid(row=1, column=1, sticky=tk.E)

    lbl_cpu_time = tk.Label(info_canvas, text="Tiempo en CPU:", bg="#f1f1f1")
    lbl_cpu_time.grid(row=2, column=1, sticky=tk.E)

    lbl_error_found = tk.Label(info_canvas, text="¿Ejecución?:", bg="#f1f1f1")
    lbl_error_found.grid(row=3, column=1, sticky=tk.E)

    ghost_frame = tk.Frame(info_canvas, bg="#f1f1f1", width=200) # To expand column 2
    ghost_frame.grid(row=0, column=2, sticky="NSEW")

    lbl_real_time_value = tk.Label(info_canvas, bg="#f1f1f1")
    lbl_real_time_value.grid(row=1, column=2, sticky=tk.W)

    lbl_cpu_time_value = tk.Label(info_canvas, bg="#f1f1f1")
    lbl_cpu_time_value.grid(row=2, column=2, sticky=tk.W)

    lbl_error_found_value = tk.Label(info_canvas, bg="#f1f1f1")
    lbl_error_found_value.grid(row=3, column=2, sticky=tk.W)

    lbl_error = tk.Label(info_canvas, bg="#f1f1f1")
    lbl_error.grid(row=4, column=1, sticky=tk.W, columnspan=2)

    # 'right_frame' widgets

    button_node_1 = tk.Button(right_frame, text="Nodo 1", state=tk.DISABLED)
    button_node_1.grid(row=0, column=0, sticky=tk.W + tk.E)

    button_node_2 = tk.Button(right_frame, text="Nodo 2", state=tk.DISABLED)
    button_node_2.grid(row=0, column=1, sticky=tk.W + tk.E)

    button_node_3 = tk.Button(right_frame, text="Nodo 3", state=tk.DISABLED)
    button_node_3.grid(row=0, column=2, sticky=tk.W + tk.E)

    button_node_4 = tk.Button(right_frame, text="Nodo 4", state=tk.DISABLED)
    button_node_4.grid(row=0, column=3, sticky=tk.W + tk.E)

    fig_cpu = Figure(figsize=(8, 4), dpi=100)
    canvas_cpu = FigureCanvasTkAgg(fig_cpu, master=right_frame)
    canvas_cpu.get_tk_widget().grid(row=1, column=0, columnspan=4)

    fig_ram = Figure(figsize=(8, 4), dpi=100)
    canvas_ram = FigureCanvasTkAgg(fig_ram, master=right_frame)
    canvas_ram.get_tk_widget().grid(row=2, column=0, columnspan=4)

    root.mainloop()