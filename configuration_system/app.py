import tkinter as tk 
import tkinter.ttk as ttk
from PIL import Image as img
from PIL import ImageTk as imgtk
import os

pieces_images = {}
pieces_id = {}
current_id = None
buttons = []
selected_lbl = None

def set_image(row, column):
    global buttons
    buttons[row][column].config(image=pieces_images[pieces_id[current_id]])

def set_current_id(row, column):
    global selected_lbl
    global current_id

    current_id = 2 * row + column
    selected_lbl.config(image=pieces_images[pieces_id[current_id]])

def send_file():
    print("File sent!")

def set_initial_position():
    global buttons
    global current_id
    global selected_lbl

    current_id = 12
    selected_lbl.config(image=pieces_images[pieces_id[current_id]])

    buttons[0][0].config(image=pieces_images["blackRook"])
    buttons[0][1].config(image=pieces_images["blackKnight"])
    buttons[0][2].config(image=pieces_images["blackBishop"])
    buttons[0][3].config(image=pieces_images["blackQueen"])
    buttons[0][4].config(image=pieces_images["blackKing"])
    buttons[0][5].config(image=pieces_images["blackBishop"])
    buttons[0][6].config(image=pieces_images["blackKnight"])
    buttons[0][7].config(image=pieces_images["blackRook"])

    buttons[1][0].config(image=pieces_images["blackPawn"])
    buttons[1][1].config(image=pieces_images["blackPawn"])
    buttons[1][2].config(image=pieces_images["blackPawn"])
    buttons[1][3].config(image=pieces_images["blackPawn"])
    buttons[1][4].config(image=pieces_images["blackPawn"])
    buttons[1][5].config(image=pieces_images["blackPawn"])
    buttons[1][6].config(image=pieces_images["blackPawn"])
    buttons[1][7].config(image=pieces_images["blackPawn"])

    buttons[6][0].config(image=pieces_images["whitePawn"])
    buttons[6][1].config(image=pieces_images["whitePawn"])
    buttons[6][2].config(image=pieces_images["whitePawn"])
    buttons[6][3].config(image=pieces_images["whitePawn"])
    buttons[6][4].config(image=pieces_images["whitePawn"])
    buttons[6][5].config(image=pieces_images["whitePawn"])
    buttons[6][6].config(image=pieces_images["whitePawn"])
    buttons[6][7].config(image=pieces_images["whitePawn"])

    buttons[7][0].config(image=pieces_images["whiteRook"])
    buttons[7][1].config(image=pieces_images["whiteKnight"])
    buttons[7][2].config(image=pieces_images["whiteBishop"])
    buttons[7][3].config(image=pieces_images["whiteQueen"])
    buttons[7][4].config(image=pieces_images["whiteKing"])
    buttons[7][5].config(image=pieces_images["whiteBishop"])
    buttons[7][6].config(image=pieces_images["whiteKnight"])
    buttons[7][7].config(image=pieces_images["whiteRook"])

def clean_table():
    global buttons
    global current_id
    global selected_lbl

    current_id = 12
    selected_lbl.config(image=pieces_images[pieces_id[current_id]])
    
    for i in range(8):
        for j in range(8):
            buttons[i][j].config(image=pieces_images[pieces_id[current_id]])

def set_cell_color(cell):
    return "#efc56f" if (cell & 0b1) == 0 else "#655b3f"

def create_button(root, width, height, row, column, bg=None, a_bg=None, image=None, command=None):
    button = tk.Button(root)

    if bg != None:
        button.config(bg=bg)
    
    if a_bg != None:
        button.config(activebackground=a_bg)

    if image != None:
        button.config(image=pieces_images[image])
    
    else:
        button.config(image=pieces_images["pixel"])

    button.config(width=width, height=height, compound="c", highlightthickness=0)
    button.grid(row=row, column=column)

    if command == 1:
        button.config(
            command=lambda row = row - 1, column = column - 9 : set_image(row, column))

    elif command == 2:
        button.config(
            command=lambda row = row - 2, column = column - 17 : set_current_id(row, column))
    
    elif command == 3:
        button.config(command=send_file)
    
    elif command == 4:
        button.config(command=set_initial_position)

    elif command == 5:
        button.config(command=clean_table)

    return button

def create_image(name):
    if (name == "pixel"):
        return tk.PhotoImage(width=1, height=1)
    
    img_path = os.path.dirname(__file__)
    img_path = os.path.abspath(os.path.join(img_path, "media"))
    img_path = os.path.abspath(os.path.join(img_path, f"{name}.png"))
    image = img.open(img_path)

    resized_image = None
    if name == "bin" or name == "start":
        resized_image = image.resize((20, 20), img.ANTIALIAS)
    else:
        resized_image = image.resize((30, 30), img.ANTIALIAS)

    return imgtk.PhotoImage(resized_image)

def create_label(root, text, row, column, columnspan, sticky=None):
    label = tk.Label(root)
    label.config(text=text, bg="#f1f1f1")
    label.grid(row=row, column=column, columnspan=columnspan)

    if sticky != None:
        label.grid(sticky=sticky)

    return label

if ("__name__"):
    # init

    root = tk.Tk()
    root.title("Sistema de parametrización")
    root.config(bg="#f1f1f1")
    root.resizable(width=False, height=False)

    # pieces images

    pieces_images["pixel"] = create_image("pixel")
    pieces_images["blackPawn"] = create_image("blackPawn")
    pieces_images["blackRook"] = create_image("blackRook")
    pieces_images["blackKnight"] = create_image("blackKnight")
    pieces_images["blackBishop"] = create_image("blackBishop")
    pieces_images["blackQueen"] = create_image("blackQueen")
    pieces_images["blackKing"] = create_image("blackKing")
    pieces_images["whitePawn"] = create_image("whitePawn")
    pieces_images["whiteRook"] = create_image("whiteRook")
    pieces_images["whiteKnight"] = create_image("whiteKnight")
    pieces_images["whiteBishop"] = create_image("whiteBishop")
    pieces_images["whiteQueen"] = create_image("whiteQueen")
    pieces_images["whiteKing"] = create_image("whiteKing")

    # pieces ids

    pieces_id[0] = "blackPawn"
    pieces_id[1] = "blackRook"
    pieces_id[2] = "blackKnight"
    pieces_id[3] = "blackBishop"
    pieces_id[4] = "blackQueen"
    pieces_id[5] = "blackKing"
    pieces_id[6] = "whitePawn"
    pieces_id[7] = "whiteRook"
    pieces_id[8] = "whiteKnight"
    pieces_id[9] = "whiteBishop"
    pieces_id[10] = "whiteQueen"
    pieces_id[11] = "whiteKing"
    pieces_id[12] = "pixel"

    current_id = 12

    # chess matrix

    for i in range(8):
        buttons_temp = []

        for j in range(8):
            buttons_temp.append(
                create_button(root, 30, 30, i + 1, j + 9, 
                    bg=set_cell_color(i * 8 + j + i), a_bg="#dcdcdc", command=1))

        buttons.append(buttons_temp)

    # button pieces

    piece_00 = create_button(root, 30, 30, 2, 17, 
        image="blackPawn", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_10 = create_button(root, 30, 30, 2, 18, 
        image="blackRook", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_01 = create_button(root, 30, 30, 3, 17, 
        image="blackKnight", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_11 = create_button(root, 30, 30, 3, 18, 
        image="blackBishop", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_02 = create_button(root, 30, 30, 4, 17, 
        image="blackQueen", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_12 = create_button(root, 30, 30, 4, 18, 
        image="blackKing", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_03 = create_button(root, 30, 30, 5, 17, 
        image="whitePawn", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_13 = create_button(root, 30, 30, 5, 18, 
        image="whiteRook", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_04 = create_button(root, 30, 30, 6, 17, 
        image="whiteKnight", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_14 = create_button(root, 30, 30, 6, 18, 
        image="whiteBishop", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_05 = create_button(root, 30, 30, 7, 17, 
        image="whiteQueen", bg="#a7894d", a_bg="#dcdcdc", command=2)
    piece_16 = create_button(root, 30, 30, 7, 18, 
        image="whiteKing", bg="#a7894d", a_bg="#dcdcdc", command=2)
    
    send_button = create_button(root, 41, 20, 8, 17, bg="#dcdcdc", a_bg="#a9a9a9", command=2)
    send_button.config(text="Limpiar")
    send_button.grid(columnspan=2)

    start_image = create_image("start")
    init_button = create_button(root, 20, 20, 1, 6, bg="#dcdcdc", a_bg="#a9a9a9", command=4)
    init_button.config(image=start_image)
    init_button.grid(columnspan=2)

    reset_image = create_image("bin")
    reset_button = create_button(root, 20, 20, 1, 7, bg="#dcdcdc", a_bg="#a9a9a9", command=5)
    reset_button.config(image=reset_image)
    reset_button.grid(columnspan=2)

    # labels

    lbl_table = create_label(root, "Tablero", 0, 9, 8)
    lbl_topology = create_label(root, "Topología:", 2, 0, 1, sticky=tk.E)
    lbl_nodes = create_label(root, "Nodos:", 3, 0, 1, sticky=tk.E)
    lbl_node1 = create_label(root, "1:", 3, 1, 1, sticky=tk.E)
    lbl_node2 = create_label(root, "2:", 3, 3, 1, sticky=tk.E)
    lbl_node3 = create_label(root, "3:", 3, 5, 1, sticky=tk.E)
    lbl_node4 = create_label(root, "4:", 3, 7, 1, sticky=tk.E)
    lbl_turn = create_label(root, "Turno:", 4, 0, 1, sticky=tk.E)
    lbl_iters = create_label(root, "Iteraciones:", 5, 0, 1, sticky=tk.E)

    # checkboxes

    node1_value = tk.BooleanVar(root)
    node1_value.set(True)
    node1_checkbox = tk.Checkbutton(root, 
        variable=node1_value, state=tk.DISABLED, bg="#f1f1f1", highlightthickness=0)
    node1_checkbox.grid(row=3, column=2)
    node2_value = tk.BooleanVar(root)
    node2_checkbox = tk.Checkbutton(root, 
        variable=node2_value, bg="#f1f1f1", highlightthickness=0)
    node2_checkbox.grid(row=3, column=4)
    node3_value = tk.BooleanVar(root)
    node3_checkbox = tk.Checkbutton(root, 
        variable=node3_value, bg="#f1f1f1", highlightthickness=0)
    node3_checkbox.grid(row=3, column=6)
    node4_value = tk.BooleanVar(root)
    node4_checkbox = tk.Checkbutton(root, 
        variable=node4_value, bg="#f1f1f1", highlightthickness=0)
    node4_checkbox.grid(row=3, column=8)

    # comboboxes

    topology_combobox = ttk.Combobox(root)
    topology_combobox.configure(width=22, height=1)
    topology_combobox.grid(row=2, column=1, columnspan=8, sticky=tk.W)
    topology_combobox["values"] = ["Completamente enlazada", "Anillo", "Estrella"]
    topology_combobox.set("Completamente enlazada")
    turn_combobox = ttk.Combobox(root)
    turn_combobox.config(width=6, height=1)
    turn_combobox.grid(row=4, column=1, columnspan=8, sticky=tk.W)
    turn_combobox["values"] = ["Blanco", "Negro"]
    turn_combobox.set("Blanco")

    # entry

    iters_entry = tk.Entry(root)
    iters_entry.config(width=5, highlightthickness=0)
    iters_entry.grid(row=5, column=1, columnspan=8, sticky=tk.W)

    # send button

    send_button = create_button(root, 200, 20, 7, 0, bg="#dcdcdc", a_bg="#a9a9a9", command=3)
    send_button.config(text="Enviar archivo de configuración")
    send_button.grid(columnspan=9)

    # selected

    selected_lbl = create_label(root, "", 1, 17, 2)
    selected_lbl.config(image=pieces_images["pixel"])

    root.mainloop()