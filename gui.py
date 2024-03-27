import tkinter
import tkinter.messagebox
import customtkinter

customtkinter.set_appearance_mode(
    "Light")  # Modes: "System" (standard), "Dark", "Light"
customtkinter.set_default_color_theme(
    "blue")  # Themes: "blue" (standard), "green", "dark-blue"


class App(customtkinter.CTk):

  def __init__(self):
    super().__init__()

    # configure window
    self.title("UNCC_ECE_PROBE")
    self.geometry(f"{1100}x{580}")

    # configure grid layout (4x4)
    self.grid_columnconfigure(1, weight=1)
    self.grid_columnconfigure((2, 3), weight=1)
    self.grid_rowconfigure((0, 1, 2), weight=1)

    # create sidebar frame with widgets
    self.sidebar_frame = customtkinter.CTkFrame(self,
                                                width=140,
                                                corner_radius=0)
    self.sidebar_frame.grid(row=0, column=0, rowspan=4, sticky="nsew")
    self.sidebar_frame.grid_rowconfigure(4, weight=1)
    self.logo_label = customtkinter.CTkLabel(self.sidebar_frame,
                                             text="UNCC_ECE_PROBE",
                                             font=customtkinter.CTkFont(
                                                 size=20, weight="bold"))
    self.logo_label.grid(row=0, column=0, padx=20, pady=(20, 10))
    self.sidebar_button_1 = customtkinter.CTkButton(
        self.sidebar_frame, command=self.sidebar_button_event, text="Method 1")
    self.sidebar_button_1.grid(row=1, column=0, padx=20, pady=10)

    self.gap_avg = customtkinter.CTkLabel(master=self, height=20, width=20, text= "gap_average")

    self.sidebar_button_2 = customtkinter.CTkButton(
        self.sidebar_frame, command=self.sidebar_button_event, text="Method 2")
    self.sidebar_button_2.grid(row=2, column=0, padx=20, pady=10)



    self.appearance_mode_label = customtkinter.CTkLabel(
        self.sidebar_frame, text="Appearance Mode:", anchor="w")
    
    self.appearance_mode_label.grid(row=5, column=0, padx=20, pady=(10, 0))
    self.appearance_mode_optionemenu = customtkinter.CTkOptionMenu(
        self.sidebar_frame,
        values=["Light", "Dark", "System"],
        command=self.change_appearance_mode_event)
    self.appearance_mode_optionemenu.grid(row=6,
                                          column=0,
                                          padx=20,
                                          pady=(10, 10))
    self.scaling_label = customtkinter.CTkLabel(self.sidebar_frame,
                                                text="UI Scaling:",
                                                anchor="w")
    self.scaling_label.grid(row=7, column=0, padx=20, pady=(10, 0))
    self.scaling_optionemenu = customtkinter.CTkOptionMenu(
        self.sidebar_frame,
        values=["80%", "90%", "100%", "110%", "120%"],
        command=self.change_scaling_event)
    self.scaling_optionemenu.grid(row=8, column=0, padx=20, pady=(10, 20))

    # create main entry and button
    self.entry = customtkinter.CTkEntry(self, placeholder_text="CTkEntry")
    self.entry.grid(row=3,
                    column=1,
                    columnspan=2,
                    padx=(20, 0),
                    pady=(20, 20),
                    sticky="nsew")

    self.main_button_1 = customtkinter.CTkButton(master=self,
                                                 fg_color="transparent",
                                                 border_width=2,
                                                 text_color=("gray10",
                                                             "#DCE4EE"))
    self.main_button_1.grid(row=3,
                            column=3,
                            padx=(20, 20),
                            pady=(20, 20),
                            sticky="nsew")

    # create textbox
    self.textbox = customtkinter.CTkTextbox(self, width=250)
    self.textbox.grid(row=0,
                      column=1,
                      padx=(20, 0),
                      pady=(20, 0),
                      sticky="nsew")

    # create tabview
    self.tabview = customtkinter.CTkTabview(self, width=250)
    self.tabview.grid(row=0,
                      column=2,
                      padx=(20, 0),
                      pady=(20, 0),
                      sticky="nsew")
    self.tabview.add("Method 1")
    self.tabview.add("Method 2")
    self.tabview.tab("Method 1").grid_columnconfigure(
        0, weight=1)  # configure grid of individual tabs
    self.tabview.tab("Method 2").grid_columnconfigure(0, weight=1)

    self.optionmenu_1 = customtkinter.CTkOptionMenu(
        self.tabview.tab("Method 1"),
        dynamic_resizing=False,
        values=["Gap Average", "# of Fingers"])
    self.optionmenu_1.grid(row=0, column=0, padx=20, pady=(20, 10))
  
    self.string_input_button = customtkinter.CTkButton(
        self.tabview.tab("Method 1"),
        text="Edit Variable",
        command=self.open_input_dialog_event)
    self.string_input_button.grid(row=1, column=0, padx=20, pady=(10, 10))

    self.optionmenu_2= customtkinter.CTkOptionMenu(
      self.tabview.tab("Method 2"),
      dynamic_resizing=False,
      values=["Gap Average", "Gridline Average", "Strip Width"])
    self.optionmenu_2.grid(row=0, column=0, padx=20, pady=(20, 10))

    self.string_input_button = customtkinter.CTkButton(
      self.tabview.tab("Method 2"),
      text="Edit Variable",
      command=self.open_input_dialog_event)
    self.string_input_button.grid(row=1, column=0, padx=20, pady=(10, 10))

    # create radiobutton frame
    self.radiobutton_frame = customtkinter.CTkFrame(self)
    self.radiobutton_frame.grid(row=0,
                                column=3,
                                padx=(20, 20),
                                pady=(20, 0),
                                sticky="nsew")
    self.radio_var = tkinter.IntVar(value=0)
    self.label_radio_group = customtkinter.CTkLabel(
        master=self.radiobutton_frame, text="Testing Method:")
    self.label_radio_group.grid(row=0,
                                column=2,
                                columnspan=1,
                                padx=10,
                                pady=10,
                                sticky="")
    self.radio_button_1 = customtkinter.CTkRadioButton(
        master=self.radiobutton_frame, variable=self.radio_var, value=0, text="Method 1")
    self.radio_button_1.grid(row=1, column=2, pady=10, padx=20, sticky="n")
    self.radio_button_2 = customtkinter.CTkRadioButton(
        master=self.radiobutton_frame, variable=self.radio_var, value=1, text="Method 2")
    self.radio_button_2.grid(row=2, column=2, pady=10, padx=20, sticky="n")
  
    # create progressbar frame
    self.slider_progressbar_frame = customtkinter.CTkFrame(
        self, fg_color="transparent")
    self.slider_progressbar_frame.grid(row=1,
                                       column=1,
                                       padx=(20, 0),
                                       pady=(20, 0),
                                       sticky="nsew")
    self.slider_progressbar_frame.grid_columnconfigure(0, weight=1)
    self.slider_progressbar_frame.grid_rowconfigure(4, weight=1)
    self.progressbar_1 = customtkinter.CTkProgressBar(
        self.slider_progressbar_frame)
    self.progressbar_1.grid(row=1,
                            column=0,
                            padx=(20, 10),
                            pady=(10, 10),
                            sticky="ew")

    # create Combobox (Method 1/2)
    self.method_frame = customtkinter.CTkFrame(self)
    self.method_frame.grid(row=1,
                                    column=3,
                                    padx=(20, 10),
                                    pady=(20, 0),
                                    sticky="nsew")
    self.method_var = tkinter.IntVar(value=0)
    self.combobox = customtkinter.CTkComboBox(master=self.method_frame, values=["Method 1", "Method 2"],
                                     command=self.combobox_callback, variable=self.method_var)
    self.method_var.set("Method 1")

  # Start Button
    self.button_frame = customtkinter.CTkFrame(self)
    self.button_frame.grid(row=1,
                              column=2,
                              padx=(20, 10),
                              pady=(20, 0),
                              sticky="nsew")
    self.radio_var = tkinter.IntVar(value=0)
    self.label_button_group = customtkinter.CTkLabel(
      master=self.button_frame, text="Testing Method:")
    self.label_button_group.grid(row=1,
                              column=2,
                              columnspan=1,
                              padx=10,
                              pady=10,
                              sticky="")
    self.start_button = customtkinter.CTkButton(
      master=self.button_frame, command=self.button_start, fg_color="green", text="Start")
    self.start_button.grid(row=1, column=2, pady=20, padx=20, sticky="n")
    self.resume_button = customtkinter.CTkButton(
      master=self.button_frame, command=self.button_resume, fg_color="Orange", text="Resume")
    self.resume_button.grid(row=2, column=2, pady=20, padx=20, sticky="n")
    self.stop_button = customtkinter.CTkButton(
      master=self.button_frame, command=self.button_stop, fg_color="red", text="Stop")
    self.stop_button.grid(row=3, column=2, pady=20, padx=20, sticky="n")

  # set default values
    self.resume_button.configure(state="disabled")
    self.appearance_mode_optionemenu.set("Light")
    self.scaling_optionemenu.set("100%")
    self.progressbar_1.configure(mode="determinate",determinate_speed= 0.2)
    self.progressbar_1.start()
    self.textbox.insert(
      0.0, "Demo for UNCC_ECE_Probe.\n\n " +
      "GRAPH PENDING \n \n "
      * 4)

  def button_stop():    
        self.resume_button.configure(state="normal")
        print("STOPPED")
  def button_start():
        print("button clicked")
  def button_resume():
       print("button clicked")

    #self.radio_button_resume = customtkinter.CTkButton(
    #master=self.radiobutton_frame, command=self.button_start, fg_color="yellow", text="Resume")
    #self.radio_button_resume.grid(row=1, column=2, pady=20, padx=20, sticky="n")

  def open_input_dialog_event(self):
    dialog = customtkinter.CTkInputDialog(text="Type in Value:",
                                          title="Input Variables")
    print("Input Value:", dialog.get_input())

  def change_appearance_mode_event(self, new_appearance_mode: str):
    customtkinter.set_appearance_mode(new_appearance_mode)

  def change_scaling_event(self, new_scaling: str):
    new_scaling_float = int(new_scaling.replace("%", "")) / 100
    customtkinter.set_widget_scaling(new_scaling_float)

  def sidebar_button_event(self):
    print("sidebar_button click")

  def combobox_callback(choice):
    print("Selected Method:", choice)

if __name__ == "__main__":
  app = App()
  app.mainloop()