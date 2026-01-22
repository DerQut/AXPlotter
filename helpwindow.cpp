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
        "<p>The program was developed by Marcel Chołodecki, a BcS at the Wrocław University of Science and Technology for the Faculty of Electronics, Photonics and Microsystems as a part of the author's engineering thesis.</p>"
        "<h2>Current issues</h2>"
        "<ul>"
        "<li>\"begin stat\" and \"end stat\" not fully \"implemented\" (in this case implementation means ignoring. Commenting out \"begin stat\" and \"end stat\" calls is highly recommended)</li>"
        "<li>Device groups do not work (and possibly never will. Commenting them out is highly recommended)</li>"
        "</ul>"
        "<h2>Script syntax</h2>"
        "<p>todo</p>"
        "<h2>Python functionallity</h2>"
        "<p>The program allows functionality to be added via specially formatted comments, containing Python function calls. Such comments should be formatted as \"<span style=\"color:red\">#$</span><span style=\"color:blue\">CODE</span><span style=\"color:red\">#;</span>\", where the Python code enclosed in the comment block is executed after finishing the AIXACT Software script line preceding it. Examples of this functionality are given below:</p>"
        "<span style=\"font-family:'Courier New'\">"
        "<br>variable MyHeaterTemperature = 1200;"
        "<br><span style=\"color:red\">#$MyHeaterTemperature.setAsReadOnly()#;</span>"
        "</span>"

        "<p>The example above causes the program to display a warning when the <span style=\"font-family:'Courier New'\">MyHeaterTemperature</span> value changes. It does not, however, prevent the value from being changed, since it would cause behaviour different to AIXACT Software's compiler.</p>"

        "<hr>"

        "<span style=\"font-family:'Courier New'\">"
        "<br><span style=\"color:red\">#$maxTemp = 1000#;</span>>"
        "<br><span style=\"color:blue\">[MyTimeVar]</span> Heater.temp <span style=\"color:blue\">to</span> MyHeaterTemperature;"
        "<span style=\"color:red\"><br>#$if Heater.temp > maxTemp:#;"
        "<br>#$&nbsp;&nbsp;&nbsp;&nbsp;print(\"Max safe temperature exceeded!\")#;"
        "<br>#$&nbsp;&nbsp;&nbsp;&nbsp;sys.stderr.write(\"Max safe temperature exceeded!\")#;</span>"
        "</span>"

        "<p>In the example above, a message is written to standard output (print) and standard error (sys.stderr.write()) if, at the end of the process step, the given variable exceeds the given threshold. Both stdout and stderr are supported and displayed at the end of the run.</p>"
        "<p>When writing python code that requires indents, like \"if\", \"while\" etc., it is required to include the indentations in the comment block (four spaces after the <span style=\"color:red\">#$</span> string).</p>"
    );
    label->setWordWrap(true);

    mainVStack->addWidget(label);
    mainVStack->addStretch();

    mainWidget->setLayout(mainVStack);

    scrollArea->setWidget(mainWidget);
    this->setCentralWidget(scrollArea);

}
