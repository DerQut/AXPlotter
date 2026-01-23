#include "helpwindow.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

#include "contentview.h"

HelpWindow::HelpWindow(ContentView* parent) :
    QMainWindow(parent)
{
    this->hide();

    QWidget* mainWidget = new QWidget(parent);
    QVBoxLayout* mainVStack = new QVBoxLayout();

    QScrollArea* scrollArea = new QScrollArea(parent);
    scrollArea->setWidgetResizable(true);

    QLabel* label = new QLabel(
        "<h1>AXPlotter v1.0.0</h1>"
        "<hr>"
        "<p>AXPlotter is a Windows XP program designed to simulate time series of variable and device values based on AIXACT Software recipe scripts.</p>"
        "<p>The program was developed by Marcel Chołodecki, a BSc at the Wrocław University of Science and Technology for the Faculty of Electronics, Photonics and Microsystems as a part of the author's engineering thesis.</p>"

        "<h2>Script syntax</h2>"
        "<p>The  text below is a simplified summary of basic functionality supported by AIXACT Software. For a detailed overview, it is recommened to use the official documentation supplied by the manufacturer.</p>"
        "<h3>Variable declaration</h3>"
        "<p>Variables can be created using the following syntax: \"'variable' name '=' value;\". The keyword \"parameter\" can be used instead of \"variable\", AXPlotter treats them both the same.</p>"
        "<p>The variable name can contain digits (except for the first character), letters and underscores. AXPlotter reserves some variable names, that should never be used:</p>"
        "<ul>"
        "<li>AX_GLOBAL_TIMESTEP</li>"
        "<li>AX_STEPCOUNT</li>"
        "<li>AXVariable</li>"
        "<li>ON, on, OFF, off, OPEN, open, CLOSE, close</li>"
        "<li>file</li>"
        "<li>sin, cos, tan, exp, exp10, log, log10, sqrt</li>"
        "<li>sys, types</li>"
        "<li>if, elif, else, while, for, other Python keywords</li>"
        "</ul>"
        "<p>Additionally, it is not recommended to use variable names ending with the string \"_AXLOOPITERATOR\".</p>"
        "<p>These restrictions come from the fact that the AIXACT Software script is translated into Python during simulation.</p>"
        "<p>The value assigned to the variable can be an integer, a floating point number, or a digital constant (ON=1, OFF=0, OPEN=1, CLOSE=0)</p>"

        "<h3>Read operation</h3>"
        "<p>Contents of other files can be added to the script via the read command using the following syntax: \"'read' filename;\". The file must be present in the same directory as the main script.</p>"
        "<p>The filename can contain an extention (like \".txt\"). If no extention is given, \".epi\" is used.</p>"
        "<p>The filename cannot contain whitespace characters or more than one dot (for the extention).</p>"

        "<h3>Macros</h3>"
        "<p>Macros are premade procedures containing process steps. A macro definition contains its name followed by the macro's contents enclosed in curly brackets. Syntax: \"MyMacro { ... }\". The macro can be later invoked using its name followed by a semicolon: \"MyMacro;\". This will result in executing all of its process steps at the time of the call.</p>"
        "<p>A special type of macro is the layer macro, which is the main procedure of the recipe. The script must contain exactly one macro named \"layer\" in order to be executed.</p>"

        "<h3>Loops</h3>"
        "<p>Loops allow recipe steps enclosed in them to be executed multiple times. Syntax: loop N { .. }, where N is the amount of repetitions. It can either be an integer or a variable name.</p>"

        "<h3>Process steps</h3>"
        "<p>Process steps are lists of assignments (separated with a comma character) to be executed in pararell. There are two types of process steps:</p>"

        "<ul>"
        "<li>Time steps: \"Time message, list;\"</li>"
        "<li>Conditional steps: \"message, list 'until' condition;\"</li>"
        "</ul>"

        "<p>Time steps execute the given assignments in the given time (it can be a number of seconds expressed as an integer, a \"hh:mm:ss\" expression, or a variable name enclosed in square brackets).</p>"

        "<p>Conditional steps halt the execution of the process until the given condition becomes true. The condition can take one of three forms: a == b, a << b, a >> b. Since it is impossible to predict the amount of time needed for the condition to become true (for example due to thermal mass of heating devices), AXPlotter treats conditional steps as time steps with a length of 0 seconds.</p>"

        "<p>In both cases, the message is a string enclosed in quotation marks. It is optional and ignored by AXPlotter. AIXACT Software, during processes, displays the messages in its user interface when the associated process steps begin.</p>"

        "<p>The assignment list can contain the given structures:</p>"

        "<h4>Direct assignment</h4>"

        "<p>Direct assignment, expressed as \"name '=' value\", assigns the given value to the given variable. When used in a time step, the value is assigned for each second of its duration.</p>"

        "<h4>Linear ramp</h4>"

        "<p>Linear ramps, expressed as \"name 'to' value 'in' time\" calculates the value to be assigned each second, assuring the resulting time series will be a linear function, reaching the given value in the given time.</p>"

        "<p>For time steps, if no time is given for the linear ramp, eg. \"name 'to' value\", the given process length is used.</p>"

        "<h4>Nonlinear ramp</h4>"

        "<p>Nonlinear ramps give a mathematical function f(x) that expresses the value in time, opposed to the linear ramp, which gives the end value. The character 'x' represents the number of seconds that have passed since the start of the given step. Syntax: \"name 'with'' function ('in'' time)\". Example: \"Heater.temp with (x*30+10)\".</p>"

        "<h4>Follow</h4>"

        "<p>The follow expression allows a variable's setpoint to be bound to an expression (including another variable). Whenever the expression's value changes, so does the variable's. Examle: Heater.temp1 follow Heater.temp2+300 </p>"

        "<h3>Comments</h3>"

        "<p>Comments allow to insert additional information in the script. Any string staring with a '#' and ending with a newline character is interpreted as a comment.</p>"

        "<hr>"

        "<h2>Python functionallity</h2>"
        "<p>The program allows functionality to be added via specially formatted comments, containing Python function calls. Such comments should be formatted as \"<span style=\"color:red\">#$</span><span style=\"color:blue\">CODE</span><span style=\"color:red\">#;</span>\", where the Python code enclosed in the comment block is executed after finishing the AIXACT Software script line preceding it. Examples of this functionality are given below:</p>"
        "<span style=\"font-family:'Courier New'\">"
        "<br><span style=\"color:blue\">variable</span> MyHeaterTemperature = 1200;"
        "<br><span style=\"color:red\">#$MyHeaterTemperature.setAsReadOnly()#;</span>"
        "</span>"

        "<p>The example above causes the program to display a warning when the <span style=\"font-family:'Courier New'\">MyHeaterTemperature</span> value changes. It does not, however, prevent the value from being changed, since it would cause behaviour different to AIXACT Software's compiler.</p>"

        "<hr>"

        "<span style=\"font-family:'Courier New'\">"
        "<br><span style=\"color:red\">#$maxTemp = 1000#;</span>"
        "<br><span style=\"color:blue\">[MyTimeVar]</span> Heater.temp <span style=\"color:blue\">to</span> MyHeaterTemperature;"
        "<span style=\"color:red\"><br>#$if Heater_temp > maxTemp:#;"
        "<br>#$&nbsp;&nbsp;&nbsp;&nbsp;print(\"Max safe temperature exceeded!\")#;"
        "<br>#$&nbsp;&nbsp;&nbsp;&nbsp;sys.stderr.write(\"Max safe temperature exceeded!\")#;</span>"
        "</span>"

        "<p>In the example above, a message is written to standard output (print) and standard error (sys.stderr.write()) if, at the end of the process step, the given variable exceeds the given threshold. Both stdout and stderr are supported and displayed at the end of the run.</p>"
        "<p>When writing python code that requires indents, like \"if\", \"while\" etc., it is required to include the indentations in the comment block (four spaces after the <span style=\"color:red\">#$</span> string).</p>"
        "<p>Additionally, python code requires dot characters to be replaced with underscore character in variable/device names, since the program reformats AIXACT Software calls to not include dot characters in non-digits."

        "<hr>"
        "<h2>Text Editor</h2>"
        "<p>The text editor allows to view and edit recipe files. Files can be opened in File→Open recipe file (Ctrl+O). If the opened file cannot be read (the editor displays only \"??\"), it is recommended to open the recipe file in Notepad, copy its contents, paste into AXPlotter and click File→Save recipe file (Ctrl+S). This issue stems from formatting a mismatch. Files saved in AXPlotter can still be read normally in Notepad and other programs. </p>"

        "<p>The editor uses syntax highlighting for ease of use.</p>"

        "<hr>"
        "<h2>Graph views</h2>"
        "<p>After a finished simulation, the program will display all generated .CSV files in the \"Graphs view\" and \"Stacked graph\" views. The former one displays all time series as separate plots, the latter merges them onto a single graph. The left panel allows modifications to be made on the stacked graph by showing/hiding series (the ✓/× buttons) and applying multipliers to them. Multipliers allow displaying of series with drastically different y-value ranges. Applying a multiplier does not modify the original data.</p>"

        "<p>Both graph views have an option to modify the x range of displayed data using the sliders located at the bottom of the screen. The slider values are independent of each other between views.</p>"

        "<p>It is possible to load .CSV files directly into AXPlotter without starting a simulation, by selecting a directory containing .CSV files in File→Open .CSV directory (or via Ctrl+Shift+O).</p>"
    );
    label->setWordWrap(true);

    mainVStack->addWidget(label);
    mainVStack->addStretch();

    mainWidget->setLayout(mainVStack);

    scrollArea->setWidget(mainWidget);
    this->setCentralWidget(scrollArea);

}
