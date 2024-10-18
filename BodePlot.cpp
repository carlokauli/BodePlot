/*
    <BodePlot is a plotter for bode diagrams after entering a transfer function
    and additionally calculates phase and amplitude margins>
    <BodePlot berechnet das Bode-Diagramm nach Eingabe einer Übertragungsfunktion
    und berechnet zusätzlich Phasen- und Amplitudenreserven.>
    Copyright (C) <2024>  <Carlo Kaulbarsch> <mail: ckaulbarsch@web.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include "BodePlot.h"
#include "ui_BodePlot.h"
#include <cmath>
#include <complex>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BodePlot)
{
    ui->setupUi(this);

    // Konfiguration des Plot-Bereichs, Amplitude
    ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->yAxis);  // Graph für Amplitude hinzufügen
    ui->customPlot->yAxis->setLabel("Amplitude (dB)"); // Amplituden-Achse beschriften
    ui->customPlot->yAxis->setLabelColor(Qt::blue); // Amplituden-Achsen-Beschriftung in blau
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));  // Amplitude in blau darstellen
    // Konfiguration des Plot-Bereichs, Phase
    ui->customPlot->yAxis2->setVisible(true);  // Zweite Y-Achse sichtbar machen
    ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->yAxis2);  // Graph für Phase hinzufügen
    ui->customPlot->yAxis2->setLabel("Phase (°)"); // Phasen-Achse beschriften
    ui->customPlot->yAxis2->setLabelColor(Qt::red); // Phasen-Achsen-Beschriftung in rot
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));  // Phase in rot darstellen
    // Konfiguration des Plot-Bereichs, Frequenz
    ui->customPlot->xAxis->setLabel("Kreisfrequenz (rad/s)"); // Frequenz-Achse beschriften
    ui->customPlot->xAxis->setScaleType(QCPAxis::stLogarithmic); // Frequenzachse auf logarithmische Skalierung stellen
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog); // Logarithmische Ticker aufrufen
    logTicker->setLogBase(10);  // Logarithmus-Basis auf 10 setzen
    ui->customPlot->xAxis->setTicker(logTicker); // logarithmischen Ticker aktivieren
    ui->customPlot->xAxis->setNumberFormat("eb"); // Werte in Frequenzachse in Zehnerpotenzen anzeigen
    ui->customPlot->xAxis->setNumberPrecision(0); // Festlegen, dass "1*10^4" nur als "10^4" angezeigt wird


    // Event für den "Berechnen"-Button
    connect(ui->calculateButton, &QPushButton::clicked, this, &MainWindow::calculateBodeDiagram);
    // Event für den "Export"-Button
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::exportFunc);
    // Event für den "About"-Button
    connect(ui->aboutButton, &QPushButton::clicked, this, &MainWindow::showAbout);
}

MainWindow::~MainWindow() {
    delete ui;
}


// Funktion um Bode-Diagramm zu berechnen
void MainWindow::calculateBodeDiagram() {
    // Frequenzbereich und Anzahl der Punkte aus UI einlesen und speichern
    double freqLow = pow(10,ui->lowFreqSpinBox->value());
    double freqHigh = pow(10, ui->highFreqSpinBox->value());
    int numPoints = ui->numpointsSpinBox->value();

    // Werte der Koeffizienten aus UI einlesen und speichern
    double a0 = ui->a0SpinBox->value();
    double a1 = ui->a1SpinBox->value();
    double a2 = ui->a2SpinBox->value();
    double a3 = ui->a1SpinBox->value();
    double a4 = ui->a2SpinBox->value();
    double b0 = ui->b0SpinBox->value();
    double b1 = ui->b1SpinBox->value();
    double b2 = ui->b2SpinBox->value();
    double b3 = ui->b3SpinBox->value();
    double b4 = ui->b4SpinBox->value();

    // Vektoren und Variablen für Berechnungen erstellen
    QVector<double> frequencies(numPoints), amplitudeData(numPoints), phaseData(numPoints);
    double phaseMargin = 0;
    double gainMargin = 0;
    bool foundPhaseMargin = false;
    bool foundGainMargin = false;

    // For-Schleife um die Berechnungspunkte abzuarbeiten
    for (int i = 0; i < numPoints; ++i) {
        frequencies[i] = freqLow * pow(10, i * (log10(freqHigh / freqLow)) / numPoints); // Samplewerte in logarithmischen Abständen berechnen
        std::complex<double> s(0, frequencies[i]);  // Als komplexe Variable s = jω abspeichern

        // Übertragungsfunktion mit den Koeffizienten berechnen
        std::complex<double> numerator = a0 + a1 * s + a2 * pow(s, 2) + a3 * pow(s, 3) + a4 * pow(s, 4);
        std::complex<double> denominator = b0 + b1 * s + b2 * pow(s, 2) + b3 * pow(s, 3) + b4 * pow(s, 4);
        std::complex<double> G = numerator / denominator;;

        amplitudeData[i] = 20 * log10(abs(G));  // Amplitude in dB berechnen
        phaseData[i] = arg(G) * 180 / M_PI;     // Phase in Grad berechnen

        // Phase kontinuierlich machen, um Phasensprung bei -180° zu verhindern
        if (i > 0) {
            while (phaseData[i] - phaseData[i - 1] > 180)
            {phaseData[i] -= 360;}
            while (phaseData[i] - phaseData[i - 1] < -180)
            {phaseData[i] += 360;}
        }

        // Speichern der Amplitudenreserve bei Phase = -180°
        if (phaseData[i] <= -180 &&!foundGainMargin) { // Überprüfen ob Phase unter -180° gesunken ist und Reserve noch nicht gefunden
            gainMargin =  amplitudeData[i]; // Reserve ist negativer zugehöriger Amplitudenwert
            foundGainMargin = true; // Status auf true setzen, damit nicht weiter gesucht wird
        }
        // Speichern der Phasenreserve bei Amplitude 0 dB
        if (amplitudeData[i] <= 0 && !foundPhaseMargin) { // Überprüfen ob Amplitude unter 0dB gesunken ist und Reserve noch nicht gefunden
            phaseMargin = 180 + phaseData[i]; // Reserve ist zugehöriger Phasenwert + 180°
            foundPhaseMargin = true; // Status auf true setzen, damit nicht weiter gesucht wird
        }
    }

    // Daten in den Plot einfügen
    ui->customPlot->graph(0)->setData(frequencies, amplitudeData);
    ui->customPlot->graph(1)->setData(frequencies, phaseData);

    // Achsen neu skalieren und Plot aktualisieren
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();

    // Anzeige der Amplitudenreserve und Phasenreserve mit zwei Nachkommastellen
    if (foundGainMargin)
    {ui->gainMarginLabel->setText("Amplitudenreserve: " + QString::number(gainMargin, 'f' , 2) + " dB");}
    else {ui->gainMarginLabel->setText("Keine Amplitudenreserve gefunden"); }
    if (foundPhaseMargin)
    {ui->phaseMarginLabel->setText("Phasenreserve: " + QString::number(phaseMargin, 'f', 2) + "°");}
    else {ui->phaseMarginLabel->setText("Keine Phasenreserve gefunden");}
}


// Funktion zum Export des Plots
void MainWindow::exportFunc() {
    // Dialog zum Wählen des Dateinames und -formats starten
    QString fileName = QFileDialog::getSaveFileName(this, tr("Plot Speichern"), "" , tr("PNG (*.png);;JPEG (*.jpg);;PDF (*.pdf)"));
    if (fileName.endsWith(".png"))
    { ui->customPlot->savePng(fileName);}
    else if (fileName.endsWith(".jpg"))
    { ui->customPlot->saveJpg(fileName);}
    else if (fileName.endsWith(".pdf"))
     { ui->customPlot->savePdf(fileName);}
}


// Funktion um About-Fenster einzublenden
void MainWindow::showAbout() {
    // String mit anzuzeigendem Text erstellen
    QString copyrightText =
        "BodePlot is a plotter for bode diagrams after entering a transfer function and additionally calculates phase and amplitude margins.\n"
        "BodePlot berechnet das Bode-Diagramm nach Eingabe einer Übertragungsfunktion und berechnet zusätzlich Phasen- und Amplitudenreserven.\n"
        "Version 1.0    Copyright (C) 2024 Carlo Kaulbarsch \n"
        "mail: ckaulbarsch@web.de\n\n"
        "This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by"
        "the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\n"
        "This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of "
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.\n\n"
        "You should have received a copy of the GNU General Public License along with this program. If not, see https://www.gnu.org/licenses/.";
    // Hinweis-Box mit erstelltem String anzeigen
    QMessageBox::information(this, tr("Copyright Information"), copyrightText);
}
