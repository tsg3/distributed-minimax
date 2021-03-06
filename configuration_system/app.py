##
# @file app.py
#
# @author Esteban Campos Granados (este0111@hotmail.com)
#
# @brief Algorithm configuration system.
#
# @version 0.1
#
# @date 2021-10-29
#

# Imports
import os
import json

import tkinter as tk 
from tkinter import messagebox as msgbox
import tkinter.ttk as ttk

from PIL import Image as img
from PIL import ImageTk as imgtk

# Global variables
## Dictionary of images for cells.
pieces_images = {}
## Dictionary with dentifiers for every type of cell value (piece).
pieces_id = {}
## Identifier of selected piece type.
current_id = None
## Matrix of buttons.
buttons = []
## Selected piece type.
selected_lbl = None
## List with the values of each cell.
matrix = []
## Value of the corresponding current selected piece.
selected_value = None
## Topology's combobox widget.
topology_combobox = None
## Nodes' checkboxes boolean variables.
node1_value = None
node2_value = None
node3_value = None
node4_value = None
## Topology's combobox widget.
turn_combobox = None
## Iterations entry widget.
iters_entry = None

#Functions
def set_bit_segment(row, column, segment):
    """! Sets piece identifier into a cell in the matrix.

    @param row      Cell's row.
    @param column   Cell's column.
    @param segment  Piece identifier.

    @return None
    """
    global matrix
    matrix[row] = (matrix[row] & (0xFFFFFFFF ^ (0xF << (4 * column)))
        | (segment << (4 * column)))

def reverse_in_range(max, min, num):
    """! Reverse a number with regard to a specific range of numbers.

    @param max  Maximun value in range.
    @param min  Minimun value in range.
    @param num  Number to reverse.

    @return Number reverse in the range.

    Example: num=2 & range=[1:7] -> return 6
    """
    return (max + min) - num

def get_char(type_int):
    """! Returns the char corresponding with the piece identifier.

    1: Pawn
    2: Rook
    3: kNight
    4: Bishop
    5: Queen
    6: King

    @param type_int Piece integer Identifier.

    @return Correspondent char identifier.
    """
    if type_int == 1:
        return "P"
    elif type_int == 2:
        return "R"
    elif type_int == 3:
        return "N"
    elif type_int == 4:
        return "B"
    elif type_int == 5:
        return "Q"
    elif type_int == 6:
        return "K"
    return ""

def check_iter(string):
    """! Checks if a string can represent a positive integer.

    @param string   Amount of iterations.

    @return True if the string is a positive integer, otherwise it returns False.
    """
    try:
        value = int(string)
        if value > 0:
            return True
        return False
    except:
        return False

def check_table(table):
    """! Checks if the state of the chess board is valid.

    @param table    Chess board state.

    @return True if the state represents a valid position, otherwise it returns False.
    """
    white_kings = 0
    white_pawns = 0
    white_other = 0

    black_kings = 0
    black_pawns = 0
    black_other = 0

    for i in table:
        if i["tipo"] == "P":
            if i["jugador"] == True:
                white_pawns += 1
            else:
                black_pawns += 1
        
        elif i["tipo"] == "K":
            if i["jugador"] == True:
                white_kings += 1
            else:
                black_kings += 1

        else:
            if i["jugador"] == True:
                white_other += 1
            else:
                black_other += 1

        if white_pawns > 8 or black_pawns > 8:
            return False

        if i["tipo"] == "P" and (i["posicionX"] == 0 or i["posicionX"] == 7):
            return False
    
    if white_kings != 1 or black_kings != 1:
        return False

    if 16 < white_pawns + white_other or 16 < black_pawns + black_other:
        return False

    return True

def set_image(row, column):
    """! Sets a piece identifier and image to a specific cell on the board.

    @param row      Cell's row.
    @param column   Cell's column.

    @return None
    """
    global buttons
    global matrix

    buttons[row][column].config(image=pieces_images[pieces_id[current_id]])
    set_bit_segment(row, column, selected_value)

def set_current_id(row, column):
    """! Sets the current piece identifier used to place pieces on the board.

    @param row      Piece widget's row.
    @param column   Piece widget's column.

    @return None
    """
    global selected_lbl
    global current_id
    global selected_value

    current_id = 2 * row + column
    selected_lbl.config(image=pieces_images[pieces_id[current_id]])

    if row == 6:
        selected_value = 0
        return
    tmp = ((2 * row + column) % 6) + 1
    selected_value = tmp if row < 3 else tmp + 8

def send_file():
    """! Creates and sends the configuration file.

    This functions checks all the input entered and the validity of the board state. If
    everything's valid, it builds the configuration JSON file and sends it to the remote
    system through SSH.

    @return None
    """
    iters = iters_entry.get()
    if not check_iter(iters):
        msgbox.showerror(title="Formato", 
            message="Las iteraciones deben ser un n??mero entero positivo!")
        return

    table = []
    for i in range(8):
        i_r = reverse_in_range(7, 0, i)
        for j in range(8):
            cell_value = (matrix[i] & (0b1111 << (4 * j))) >> (4 * j)
            if cell_value == 0:
                continue

            piece = {}
            piece["tipo"] = get_char(cell_value & 7)
            piece["posicionX"] = i_r
            piece["posicionY"] = j
            piece["jugador"] = True if (cell_value & 8) >> 3 == 1 else False
            table.append(piece)
    
    if not check_table(table):
        msgbox.showerror(title="Tablero", 
            message="El estado del tablero no es v??lido!")
        return

    path = os.path.dirname(__file__)
    path = os.path.abspath(os.path.join(path, "file"))
    file = open(path, 'w')

    data = {}
    data["topologia"] = topology_combobox.get()
    data["nodos"] = [
        node1_value.get(), node2_value.get(), node3_value.get(), node4_value.get()]
    data["tablero"] = table
    data["turno"] = True if turn_combobox.get() == "Blanco" else False
    data["iteraciones"] = int(iters)

    file.write(json.dumps(data))

    file.close()

def set_initial_position():
    """! Establishes the common initial position on the board.

    This function erases the current state of the board's matrix, and replaces it with
    the common initial or first position of a chess game. 

    @return None
    """
    global buttons
    global current_id
    global selected_lbl
    global matrix
    global selected_value

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
    set_bit_segment(0, 0, 10)
    set_bit_segment(0, 1, 11)
    set_bit_segment(0, 2, 12)
    set_bit_segment(0, 3, 13)
    set_bit_segment(0, 4, 14)
    set_bit_segment(0, 5, 12)
    set_bit_segment(0, 6, 11)
    set_bit_segment(0, 7, 10)

    buttons[1][0].config(image=pieces_images["blackPawn"])
    buttons[1][1].config(image=pieces_images["blackPawn"])
    buttons[1][2].config(image=pieces_images["blackPawn"])
    buttons[1][3].config(image=pieces_images["blackPawn"])
    buttons[1][4].config(image=pieces_images["blackPawn"])
    buttons[1][5].config(image=pieces_images["blackPawn"])
    buttons[1][6].config(image=pieces_images["blackPawn"])
    buttons[1][7].config(image=pieces_images["blackPawn"])
    set_bit_segment(1, 0, 9)
    set_bit_segment(1, 1, 9)
    set_bit_segment(1, 2, 9)
    set_bit_segment(1, 3, 9)
    set_bit_segment(1, 4, 9)
    set_bit_segment(1, 5, 9)
    set_bit_segment(1, 6, 9)
    set_bit_segment(1, 7, 9)

    for i in range(2, 6):
        for j in range(8):
            buttons[i][j].config(image=pieces_images["pixel"])
        matrix[i] = 0

    buttons[6][0].config(image=pieces_images["whitePawn"])
    buttons[6][1].config(image=pieces_images["whitePawn"])
    buttons[6][2].config(image=pieces_images["whitePawn"])
    buttons[6][3].config(image=pieces_images["whitePawn"])
    buttons[6][4].config(image=pieces_images["whitePawn"])
    buttons[6][5].config(image=pieces_images["whitePawn"])
    buttons[6][6].config(image=pieces_images["whitePawn"])
    buttons[6][7].config(image=pieces_images["whitePawn"])

    set_bit_segment(6, 0, 1)
    set_bit_segment(6, 1, 1)
    set_bit_segment(6, 2, 1)
    set_bit_segment(6, 3, 1)
    set_bit_segment(6, 4, 1)
    set_bit_segment(6, 5, 1)
    set_bit_segment(6, 6, 1)
    set_bit_segment(6, 7, 1)

    buttons[7][0].config(image=pieces_images["whiteRook"])
    buttons[7][1].config(image=pieces_images["whiteKnight"])
    buttons[7][2].config(image=pieces_images["whiteBishop"])
    buttons[7][3].config(image=pieces_images["whiteQueen"])
    buttons[7][4].config(image=pieces_images["whiteKing"])
    buttons[7][5].config(image=pieces_images["whiteBishop"])
    buttons[7][6].config(image=pieces_images["whiteKnight"])
    buttons[7][7].config(image=pieces_images["whiteRook"])
    set_bit_segment(7, 0, 2)
    set_bit_segment(7, 1, 3)
    set_bit_segment(7, 2, 4)
    set_bit_segment(7, 3, 5)
    set_bit_segment(7, 4, 6)
    set_bit_segment(7, 5, 4)
    set_bit_segment(7, 6, 3)
    set_bit_segment(7, 7, 2)

    selected_value = 0

def clean_table():
    """! Cleans the current position of the board.

    This function erases the current state of the board's matrix.

    @return None
    """
    global buttons
    global current_id
    global selected_lbl
    global matrix
    global selected_value

    current_id = 12
    selected_lbl.config(image=pieces_images[pieces_id[current_id]])
    
    for i in range(8):
        for j in range(8):
            buttons[i][j].config(image=pieces_images[pieces_id[current_id]])
        matrix[i] = 0
    selected_value = 0

def set_cell_color(cell):
    """! Set's the color of a cell on the board.

    @param cell Cell's position computed with its X and Y position.

    @return Hex value of the corresponding color.
    """
    return "#efc56f" if (cell & 0b1) == 0 else "#655b3f"

def create_button(root, width, height, row, column, 
    bg=None, a_bg=None, image=None, command=None):
    """! Creates a custom Tkinter button widget.

    @param root     Tkinter container where the button will be placed.
    @param width    Button's width.
    @param height   Button's height.
    @param row      Button's X position.
    @param column   Button's Y position.
    @param bg       Button's background color.          Default: None
    @param a_bg     Button's active background color.   Default: None
    @param image    Button's image.                     Default: None
    @param command  Button's custom command.            Default: None

    @return Tkinter button widget.
    """
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
    """! Loads and creates a Tkinter piece image for the buttons.

    @param name Filename of the '.png' file.

    @return A Tkinter image.
    """
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
    """! Creates and configures a custom Tkinter labels.

    @param root         Container of the label.
    @param text         Label's text string value.
    @param row          Label's X position.
    @param column       Label's Y position.
    @param columnspan   Label's grid columnspan value.
    @param sticky       Label's sticky value. Default: None.

    @return A Tkinter label.
    """
    label = tk.Label(root)
    label.config(text=text, bg="#f1f1f1")
    label.grid(row=row, column=column, columnspan=columnspan)

    if sticky != None:
        label.grid(sticky=sticky)

    return label

if __name__ == "__main__":
    """! Main. Initializes the program. """
    # init

    root = tk.Tk()
    root.title("Sistema de parametrizaci??n")
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

    matrix = [0] * 8
    selected_value = 0b0000

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
    lbl_topology = create_label(root, "Topolog??a:", 2, 0, 1, sticky=tk.E)
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
    send_button.config(text="Enviar archivo de configuraci??n")
    send_button.grid(columnspan=9)

    # selected

    selected_lbl = create_label(root, "", 1, 17, 2)
    selected_lbl.config(image=pieces_images["pixel"])

    root.mainloop()