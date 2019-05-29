#include "GraphWidget.h"
#include "jkqtplotter/jkqtpgraphs.h"
#include "Application.h"
#include "QtAwesome/QtAwesome.h"
#include "PTFWidget.h"
#include <QVBoxLayout>
#include <QProgressDialog>
#include <QMenu>
#include <QLineEdit>
#include <QFileInfo>
#include <QDir>
#include <QVector>

int GraphWidget::count = 0;

GraphWidget::GraphWidget()
{
	plotDate = new JKQTPlotter(this);
	plotDate->deregisterMouseDragAction(Qt::LeftButton, Qt::NoModifier);
	plotDate->deregisterMouseDragAction(Qt::LeftButton, Qt::ControlModifier);
	plotDate->registerMouseDragAction(Qt::LeftButton, Qt::ControlModifier, jkqtpmdaPanPlotOnMove);
	plotDate->registerMouseDragAction(Qt::LeftButton,Qt::NoModifier, jkqtpmdaZoomByRectangle);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(plotDate);
	setLayout(layout);
	++count;
}


GraphWidget::~GraphWidget()
{
}

void GraphWidget::setData(const QVector<double>& X, const QVector<double>& Y)
{
	JKQTPDatastore* ds = plotDate->getDatastore();
	size_t cX = ds->addCopiedColumn(X,"time");
	size_t cY = ds->addCopiedColumn(Y, "dataY");
	JKQTPXYLineGraph *graph = new JKQTPXYLineGraph(plotDate);
	graph->setXColumn(cX);
	graph->setYColumn(cY);
	plotDate->addGraph(graph);
}

void GraphWidget::setPTF(const QString & path, const QStringList &name,const QVector<int> & str)
{
	PTFile ptfile(path);
	QList<QVector<float>> varlist = ptfile.GetPlot(str);
	if (varlist.isEmpty()) {
		return;
	}
	JKQTPDatastore* ds = plotDate->getDatastore();
	size_t cX = ds->addCopiedColumn(varlist.first(), "time");
	size_t cY;
	QProgressDialog progressDialog(this);
	int maxp = varlist.length();
	progressDialog.setCancelButtonText(tr("&Cancel"));
	progressDialog.setRange(1, maxp);
	progressDialog.setWindowModality(Qt::WindowModal);
	progressDialog.setMinimumDuration(1000);
	for (int i = 1; i < maxp; i++)
	{
		progressDialog.setValue(i);
		progressDialog.setLabelText(tr("drawing graph %1 of %2.").arg(i).arg(maxp - 2));
		if (progressDialog.wasCanceled())
			break;
		cY = ds->addCopiedColumn(varlist.at(i), name.at(i-1));
		JKQTPXYLineGraph *graph = new JKQTPXYLineGraph(plotDate);
		//graph->setLineWidth(1.0);
		graph->setSymbolSize(0.0);
		graph->setXColumn(cX);
		graph->setYColumn(cY);
		graph->setTitle(name.at(i - 1));
		plotDate->addGraph(graph);
	}
	plotDate->getXAxis()->setAxisLabel("time(s)");
	plotDate->zoomToFit();
	progressDialog.setValue(maxp);
	QApplication::restoreOverrideCursor();
}
