#include "mainwindow.h"
#include "qcustomplot.h"
//#include "QCPAxis"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    srand(QDateTime::currentDateTime().toTime_t()); //gerçek zaman ayarla
    ui->setupUi(this);

    ui->qcustomplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);
      ui->qcustomplot->xAxis->setRange(-8, 8);
      ui->qcustomplot->yAxis->setRange(-5, 5);
      ui->qcustomplot->axisRect()->setupFullAxesBox();

      ui->qcustomplot->plotLayout()->insertRow(0);
      QCPTextElement *title = new QCPTextElement(ui->qcustomplot, "Qt Merhaba", QFont("sans", 17, QFont::Bold));
      ui->qcustomplot->plotLayout()->addElement(0, 0, title);


      ui->qcustomplot->xAxis->setLabel("x Axis");
        ui->qcustomplot->yAxis->setLabel("y Axis");
        ui->qcustomplot->legend->setVisible(true);
        QFont legendFont = font();
        legendFont.setPointSize(10);
        ui->qcustomplot->legend->setFont(legendFont);
        ui->qcustomplot->legend->setSelectedFont(legendFont);
        ui->qcustomplot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items

        addRandomGraph();
        addRandomGraph();
        addRandomGraph();
        addRandomGraph();
        ui->qcustomplot->rescaleAxes();


        // bazı eksen seçimlerini birbirine bağlayan yuvayı bağlayın (özellikle zıt eksenleri):
          connect(ui->qcustomplot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
          // bir eksen seçildiğinde, yalnızca o yönün sürüklenip yakınlaştırılabilmesini sağlayan yuvaları bağlayın:
          connect(ui->qcustomplot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
          connect(ui->qcustomplot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

          // alt ve sol eksenlerin aralıklarını üst ve sağ eksenlere aktarmasını sağlayın:
          connect(ui->qcustomplot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->qcustomplot->xAxis2, SLOT(setRange(QCPRange)));
          connect(ui->qcustomplot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->qcustomplot->yAxis2, SLOT(setRange(QCPRange)));

          // bazı etkileşim yuvalarını bağlayın:
          connect(ui->qcustomplot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
          connect(ui->qcustomplot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));
          connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));

          // bir grafiğe tıklandığında durum çubuğunda bir mesaj gösteren yuvayı bağla:
          connect(ui->qcustomplot, SIGNAL(plottableClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*,int)));

          // içerik menüsü açılır penceresi için politikayı ayarla ve bağlantı yuvası:
          ui->qcustomplot->setContextMenuPolicy(Qt::CustomContextMenu);
          connect(ui->qcustomplot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

/*
          //make left and bottom axes transfer their ranges to right and top axes:sss
          connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
          connect(qcustomplot->yAxis, SIGNAL(rangeChanged(QCPRange)), qcustomplot->yAxis2, SLOT(setRange(QCPRange)));

          // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
          connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
          dataTimer.start(0); // Interval 0 means to refresh as fast as possible
          realtimeDataSlot, called by timer;

*/




    ui->qcustomplot->addGraph();
    ui->qcustomplot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->qcustomplot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->qcustomplot->xAxis->setLabel("whilone X");
    ui->qcustomplot->yAxis->setLabel("whilone Y");
    ui->qcustomplot->xAxis->setRange(-6000,6000);

    ui->qcustomplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    QVector <double> x ={1,2,3,4,5},y={4,6,8,2,5};

    ui->qcustomplot->graph(0)->setData(x,y);
    ui->qcustomplot->rescaleAxes();
    ui->qcustomplot->replot();
    ui->qcustomplot->update();


/*
   // Setup function:
    QCustomPlot->addGraph(); // blue line
    QCustomPlot(->graph(0)->setPen(QPen(QColor(40, 110, 255))));
    customPlot->addGraph(); // red line
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(-1.2, 1.2);


    //make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
    realtimeDataSlot, called by timer,

        //realtimeDataSlot, called by timer:
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->qcustomplot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
      ui->qcustomplot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->qcustomplot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->qcustomplot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusbar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->qcustomplot->graph(0)->data()->size()+ui->qcustomplot->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }

*/
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Üzerine çift tıklayarak arsa başlığını ayarlayın
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->qcustomplot->replot();
    }
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Üzerine çift tıklayarak bir eksen etiketi ayarlayın
  if (part == QCPAxis::spAxisLabel) // // yalnızca gerçek eksen etiketi tıklandığında tepki verir, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->qcustomplot->replot();
    }
  }
}



void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->qcustomplot->replot();
    }
  }
}


void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->qcustomplot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->qcustomplot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->qcustomplot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->qcustomplot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->qcustomplot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->qcustomplot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->qcustomplot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->qcustomplot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->qcustomplot->graph(i);
    QCPPlottableLegendItem *item = ui->qcustomplot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}



void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->qcustomplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeZoom(ui->qcustomplot->xAxis->orientation());
  else if (ui->qcustomplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->qcustomplot->axisRect()->setRangeZoom(ui->qcustomplot->yAxis->orientation());
  else
    ui->qcustomplot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::addRandomGraph()
{
  int n = 50; // number of points in graph
  double xScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double yScale = (rand()/(double)RAND_MAX + 0.5)*2;
  double xOffset = (rand()/(double)RAND_MAX - 0.5)*4;
  double yOffset = (rand()/(double)RAND_MAX - 0.5)*10;
  double r1 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r2 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r3 = (rand()/(double)RAND_MAX - 0.5)*2;
  double r4 = (rand()/(double)RAND_MAX - 0.5)*2;
  QVector<double> x(n), y(n);
  for (int i=0; i<n; i++)
  {
    x[i] = (i/(double)n-0.5)*10.0*xScale + xOffset;
    y[i] = (qSin(x[i]*r1*5)*qSin(qCos(x[i]*r2)*r4*3)+r3*qCos(qSin(x[i])*r4*2))*yScale + yOffset;
  }

  ui->qcustomplot->addGraph();
  ui->qcustomplot->graph()->setName(QString("New graph %1").arg(ui->qcustomplot->graphCount()-1));
  ui->qcustomplot->graph()->setData(x, y);
  ui->qcustomplot->graph()->setLineStyle((QCPGraph::LineStyle)(rand()%5+1));
  if (rand()%100 > 50)
    ui->qcustomplot->graph()->setScatterStyle(QCPScatterStyle((QCPScatterStyle::ScatterShape)(rand()%14+1)));
  QPen graphPen;
  graphPen.setColor(QColor(rand()%245+10, rand()%245+10, rand()%245+10));
  graphPen.setWidthF(rand()/(double)RAND_MAX*2+1);
  ui->qcustomplot->graph()->setPen(graphPen);
  ui->qcustomplot->replot();
}

void MainWindow::removeSelectedGraph()
{
  if (ui->qcustomplot->selectedGraphs().size() > 0)
  {
    ui->qcustomplot->removeGraph(ui->qcustomplot->selectedGraphs().first());
    ui->qcustomplot->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  ui->qcustomplot->clearGraphs();
  ui->qcustomplot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->qcustomplot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (ui->qcustomplot->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->qcustomplot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }

  menu->popup(ui->qcustomplot->mapToGlobal(pos));
}

void MainWindow::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->qcustomplot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->qcustomplot->replot();
    }
  }
}

/*
void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
  // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
  // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
  double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
  QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
  ui->statusBar->showMessage(message, 2500);
}

*/
