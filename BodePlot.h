/*
    <BodePlot is a plotter for bode diagrams after entering a transfer function
    and additionally calculates phase and amplitude margins.>
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

#ifndef BODEPLOT_H
#define BODEPLOT_H

#include <QMainWindow>


namespace Ui {
class BodePlot;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Funktion um Bode-Diagramm zu berechnen deklarieren
    void calculateBodeDiagram();

    // Funktion um About-Fenster einzublenden deklarieren
    void showAbout();

    // Funktion zum Export des Plots deklarieren
    void exportFunc();


private:
    Ui::BodePlot *ui;

};

#endif // BODEPLOT_H
