import json
from logging import exception
import tkinter as tk

from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
from numpy.core.fromnumeric import size

button_node_1 = None
button_node_2 = None
button_node_3 = None
button_node_4 = None

lbl_real_time_value = None
lbl_cpu_time_value = None
lbl_error_found_value = None
lbl_error = None

current_plts_pos = None
nodes_info = None
plts_available = [None, None, None, None]

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

def create_plots(id):
    fig_cpu.clear()
    data_cpu = get_data(nodes_info[f"nodo_{id}"]["cpu"])
    plt = fig_cpu.add_subplot()
    plt.plot(data_cpu[0], data_cpu[1])
    plt.grid(True)
    plt.set_title(f"Consumo de CPU en el tiempo del Nodo {id}")
    plt.set_ylabel("Consumo de CPU (%)")
    plt.set_xlabel("Tiempo (s)")
    fig_cpu.tight_layout()

    fig_ram.clear()
    data_ram = get_data(nodes_info[f"nodo_{id}"]["ram"])
    plt = fig_ram.add_subplot()
    plt.plot(data_ram[0], data_ram[1])
    plt.grid(True)
    plt.set_title(f"Uso de RAM en el tiempo del Nodo {id}")
    plt.set_ylabel("Uso de RAM (kB)")
    plt.set_xlabel("Tiempo (s)")
    fig_ram.tight_layout()

    canvas_cpu.draw()
    canvas_ram.draw()

def set_figures(report):
    global current_plts_pos
    global nodes_info
    global plts_available

    nodes_info = report["nodos"]
    current_plts_pos = None

    data_cpu = get_data(report["nodos"]["nodo_1"]["cpu"])
    plt = fig_cpu.add_subplot()
    plt.plot(data_cpu[0], data_cpu[1])
    plt.grid(True)
    plt.set_title("Consumo de CPU en el tiempo del Nodo 1")
    plt.set_ylabel("Consumo de CPU (%)")
    plt.set_xlabel("Tiempo (s)")
    plt.remove()
    fig_cpu.axes.append(plt)

    try:
        _ = nodes_info["nodo_1"]
        plts_available[0] = tk.ACTIVE
    except:
        plts_available[0] = tk.DISABLED

    try:
        _ = nodes_info["nodo_2"]
        plts_available[1] = tk.ACTIVE
    except:
        plts_available[1] = tk.DISABLED

    try:
        _ = nodes_info["nodo_3"]
        plts_available[2] = tk.ACTIVE
    except:
        plts_available[2] = tk.DISABLED

    try:
        _ = nodes_info["nodo_4"]
        plts_available[3] = tk.ACTIVE
    except:
        plts_available[3] = tk.DISABLED

    button_node_1.configure(state=plts_available[0])
    button_node_2.configure(state=plts_available[1])
    button_node_3.configure(state=plts_available[2])
    button_node_4.configure(state=plts_available[3])

    current_pos = 0
    while current_pos < 4:
        if plts_available[0] == tk.ACTIVE:
            break
        current_pos += 1

    change_to_plot(current_pos + 1)

    canvas_cpu.draw()
    canvas_ram.draw()

def set_current_report(report):
    string_real_time = None
    string_cpu_time = None
    boolean_error = None
    string_error = None

    try:
        string_real_time = f"{report['tiempo_real']} s"
        string_cpu_time = f"{report['tiempo_cpu']} s"
        boolean_error = report['error_encontrado']
        string_error = f"{report['error']}"
    except Exception as e:
        print(e)
        return tk.messagebox.showerror(
            title="Parseo fallido", 
            message="Hubo un error al parsear el archivo!"
        )

    lbl_real_time_value.configure(text=string_real_time)
    lbl_cpu_time_value.configure(text=string_cpu_time)

    if boolean_error:
        lbl_error_found_value.configure(text="Error", fg="#FF0000")
        lbl_error.configure(text=string_error)
    else:
        lbl_error_found_value.configure(text="Exitosa", fg="#00FF00")
        lbl_error.configure(text="")

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

    report = None
    try:
        report = open_report_as_dict(filename)
    except Exception as e:
        return tk.messagebox.showerror(
            title="Error", 
            message=f"El archivo '{filename}' no es un archivo válido!\nIntentelo de nuevo"
        )

    return set_current_report(report)

def open_report_as_dict(path):
    with open(path) as json_file:
        data = json.load(json_file)
    
    return data

def change_to_plot_aux(id):
    global current_plts_pos

    create_plots(id)
    current_plts_pos = id

def change_to_plot(id):
    if id == current_plts_pos:
        return

    elif id == 1 and plts_available[0] == tk.ACTIVE:
        change_to_plot_aux(1)

    elif id == 2 and plts_available[1] == tk.ACTIVE:
        change_to_plot_aux(2)

    elif id == 3 and plts_available[2] == tk.ACTIVE:
        change_to_plot_aux(3)

    elif id == 4 and plts_available[3] == tk.ACTIVE:
        change_to_plot_aux(4)

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

    button_node_1 = tk.Button(right_frame, text="Nodo 1", state=tk.DISABLED, 
        command = lambda: change_to_plot(1))
    button_node_1.grid(row=0, column=0, sticky=tk.W + tk.E)

    button_node_2 = tk.Button(right_frame, text="Nodo 2", state=tk.DISABLED,
        command = lambda: change_to_plot(2))
    button_node_2.grid(row=0, column=1, sticky=tk.W + tk.E)

    button_node_3 = tk.Button(right_frame, text="Nodo 3", state=tk.DISABLED,
        command = lambda: change_to_plot(3))
    button_node_3.grid(row=0, column=2, sticky=tk.W + tk.E)

    button_node_4 = tk.Button(right_frame, text="Nodo 4", state=tk.DISABLED,
        command = lambda: change_to_plot(4))
    button_node_4.grid(row=0, column=3, sticky=tk.W + tk.E)

    fig_cpu = Figure(figsize=(8, 4), dpi=100)
    canvas_cpu = FigureCanvasTkAgg(fig_cpu, master=right_frame)
    canvas_cpu.get_tk_widget().grid(row=1, column=0, columnspan=4)

    fig_ram = Figure(figsize=(8, 4), dpi=100)
    canvas_ram = FigureCanvasTkAgg(fig_ram, master=right_frame)
    canvas_ram.get_tk_widget().grid(row=2, column=0, columnspan=4)

    root.mainloop()