#include "mainwindow.h"

#include <QComboBox>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QToolButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolbars();
    createToolBox();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);

//    ------------------------------

    scene = new DiagramScene(this);
    view = new QGraphicsView(scene);

    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("Redactor"));
}

//void MainWindow::sceneScaleChanged(const QString &scale)
//{
//    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
////    QTransform oldMatrix = view->transform();
//    view->resetTransform();
////    view->translate(oldMatrix.dx(), oldMatrix.dy());
//    view->scale(newScale, newScale);
//}

//This method create action for menu bar and tool bar
void MainWindow::createActions()
{
    createAction = new QAction(/*QIcon("."),*/"Create", this);
    createAction->setShortcut(tr("Ctrl+N"));
    createAction->setStatusTip("Creating a new diagram...");
//    connect()

    openAction = new QAction("Open", this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip("Opening diagram");

    saveAction = new QAction("Save", this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip("Saving...");

    previewAction = new QAction("Preview PDF", this);
    connect(previewAction, &QAction::triggered, this, &MainWindow::opentPreviewDialog);

    exportPdfAction = new QAction("Export PDF", this);
    exportPngAction = new QAction("Export PNG", this);

    aboutAction = new QAction("About this programm", this);
}

//Creating a menu bar
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction(createAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(previewAction);
    exportMenu = fileMenu->addMenu("Export");
    exportMenu->addAction(exportPdfAction);
    exportMenu->addAction(exportPngAction);

    aboutMenu = menuBar()->addMenu("Help");
    aboutMenu->addAction(aboutAction);

}

//This function creats tool bars
void MainWindow::createToolbars()
{
    //Creating fit scene width into view button
    QToolButton *fitWidthButton = new QToolButton;
    fitWidthButton->setIcon(QIcon("./assets/fitWidth.png"));
    connect(fitWidthButton, &QToolButton::clicked, this, [&] {
        view->fitInView(scene->paper->rect().
                        adjusted(-50,-50,50,50-scene->paper->rect().height()),
                        Qt::KeepAspectRatio);
        setComboCurScale();
    });
    //Creating fit scene page into view button
    QToolButton *fitPageButton = new QToolButton;
    fitPageButton->setIcon(QIcon("./assets/fitPage.png"));
    connect(fitPageButton, &QToolButton::clicked, this, [&] {
        view->fitInView(scene->paper->rect(),
                        Qt::KeepAspectRatio);
        setComboCurScale();
    });
    //Adding fit tool bar
    fitToolBar = addToolBar(tr("Fit type"));
    fitToolBar->addWidget(fitWidthButton);
    fitToolBar->addWidget(fitPageButton);

    //Creating scene scale tool box
    sceneScaleCombo = new QComboBox;
    sceneScaleCombo->setEditable(true);
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
//    connect(sceneScaleCombo, &QComboBox::currentTextChanged, this, &MainWindow::sceneScaleChanged);
    connect(sceneScaleCombo, &QComboBox::currentTextChanged, this, [&] (const QString &scale) {
        double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
        view->resetTransform();
        view->scale(newScale, newScale);
    });
    //Create increase scale button
    QToolButton *increaseScaleButton = new QToolButton;
    increaseScaleButton->setIcon(QIcon("./assets/increaseScale.png"));
    connect(increaseScaleButton, &QToolButton::clicked, this, [&] {
        view->scale(1.15, 1.15);
        setComboCurScale();
    });
    //Create decrease scale button
    QToolButton *decreaseScaleButton = new QToolButton;
    decreaseScaleButton->setIcon(QIcon("./assets/decreaseScale.png"));
    connect(decreaseScaleButton, &QToolButton::clicked, this, [&] {
        view->scale(0.85, 0.85);
        setComboCurScale();
    });
    scaleToolBar = addToolBar(tr("Scale"));
    scaleToolBar->addWidget(sceneScaleCombo);
    scaleToolBar->addWidget(increaseScaleButton);
    scaleToolBar->addWidget(decreaseScaleButton);

    //Creating cursor button
    QToolButton *cursorButton = new QToolButton;
    cursorButton->setCheckable(true);
    cursorButton->setChecked(true);
    cursorButton->setIcon(QIcon(":/assets/cursor.png"));
//    connect(cursorButton, &QToolButton::clicked, this, [&] {

//    });
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon("C:/Users/Daniil/Documents/Coursework_TRPS/assets/diagonal-line-icon.png"));
//    connect(linePointerButton, &QToolButton::clicked, this, [&] {
//                                                            this->view->scale(0.8, 0.8);
//                                                         });

//    pointerTypeGroup = new QButtonGroup(this);
//    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
//    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
//    connect(pointerTypeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
//            this, &MainWindow::pointerGroupClicked);


    defaultToolBar = addToolBar(tr("Pointer type"));
    defaultToolBar->addWidget(cursorButton);
    defaultToolBar->addWidget(linePointerButton);
//    defaultToolBar->addWidget(sceneScaleCombo);
}

void MainWindow::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
//    connect(buttonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
//            this, &MainWindow::buttonGroupClicked);
    QGridLayout *layout = new QGridLayout;
//    layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createDiagramItemButton(tr("Process"), DiagramItem::X/*Step*/),0, 1);
//    layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 1, 0);

//    QToolButton *textButton = new QToolButton;
//    textButton->setCheckable(true);
//    buttonGroup->addButton(textButton, InsertTextButton);
//    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
//    textButton->setIconSize(QSize(50, 50));
//    QGridLayout *textLayout = new QGridLayout;
//    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
//    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
//    QWidget *textWidget = new QWidget;
//    textWidget->setLayout(textLayout);
//    layout->addWidget(textWidget, 1, 1);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

//    backgroundButtonGroup = new QButtonGroup(this);
//    connect(backgroundButtonGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
//            this, &MainWindow::backgroundButtonGroupClicked);

//    QGridLayout *backgroundLayout = new QGridLayout;
//    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"),
//                                                           ":/images/background1.png"), 0, 0);
//    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"),
//                                                           ":/images/background2.png"), 0, 1);
//    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"),
//                                                           ":/images/background3.png"), 1, 0);
//    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"),
//                                                           ":/images/background4.png"), 1, 1);

//    backgroundLayout->setRowStretch(2, 10);
//    backgroundLayout->setColumnStretch(2, 10);

//    QWidget *backgroundWidget = new QWidget;
//    backgroundWidget->setLayout(backgroundLayout);

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    //    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}

void MainWindow::setComboCurScale()
{
    sceneScaleCombo->setCurrentText(QString::number(
                                        view->transform().m11()*100, 'f', 1 )+"%");
}

QWidget *MainWindow::createDiagramItemButton(const QString &text, DiagramItem::DiagramType type)
{
    DiagramItem item;
    QIcon icon(item.icon());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

void MainWindow::opentPreviewDialog()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog *dialog = new QPrintPreviewDialog(&printer);

    printer.setPageSize((QPrinter::PageSize)scene->paper->size());
    printer.setOrientation((QPrinter::Orientation)scene->paper->orientation());

    connect(dialog, &QPrintPreviewDialog::paintRequested, [&](QPrinter *previewPrinter) {
//        document->print(previewPrinter); //QTextDocument *document = new QTextDocument();
        QPainter p(previewPrinter);
//        scene->setSceneRect(scene->itemsBoundingRect());
//        scene->paper->setVisible(false);
        scene->setSceneRect(scene->paper->rect());
        scene->render(&p);
        p.end();
    });

    dialog->exec();

    delete dialog;
}
