#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QButtonGroup>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMenu>
#include <QToolBar>
#include <QToolBox>
#include <diagramscene.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

//private slots:
//    void sceneScaleChanged(const QString &str);

private:
    void createActions();
    void createMenus();
    void createToolbars();
    void createToolBox();
    void setComboCurScale();

    QToolButton *createToolBoxButton(DiagramItem &item, int id);

    void opentPreviewDialog();

    QMenu *fileMenu;
    QMenu *exportMenu;
    QMenu *aboutMenu;

    QToolBar *fitToolBar;
    QToolBar *scaleToolBar;
    QToolBar *currentToolBar;
    QToolBar *defaultToolBar;
    QToolBar *textToolBar;

    QButtonGroup *actionTypeGroup;

    QAction *createAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *previewAction;
    QAction *exportPngAction;
    QAction *exportPdfAction;
    QAction *aboutAction;

    QComboBox *sceneScaleCombo;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;

    DiagramScene *scene;
    QGraphicsView *view;

};
#endif // MAINWINDOW_H
