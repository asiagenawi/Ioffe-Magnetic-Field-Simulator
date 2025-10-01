
double PI = 3.1415926;
double mu0 = 4.*PI*1e-7;
double radius = 0.18; 
int nWires = 8;   
int npixels = 100; 

double DistanceBetweenTwoPoints(double x1, double y1, double x2, double y2){
return pow((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1), 0.5); 
}

double BiotSavartField(double wireCurrent, double x1, double y1, double x2, double y2){
double distance = DistanceBetweenTwoPoints(x1, y1, x2, y2);
double bField = mu0  * wireCurrent / (distance *2.0); 
printf("B-Field = %g\n", bField);
return bField;
}

double bFieldY(double wireCurrent, double x1, double y1, double x2, double y2){ 
double distance = DistanceBetweenTwoPoints(x1, y1, x2, y2);
double bField = BiotSavartField(wireCurrent, x1, y1, x2, y2);
double sineTheta = (y2 - y1)/ distance;
double bFieldY = sineTheta * bField;
printf("B-Field Y Component  = %g\n", bFieldY);
return bFieldY;
} 

double bFieldX(double wireCurrent, double x1, double y1, double x2, double y2){
double distance = DistanceBetweenTwoPoints(x1, y1, x2, y2);
double bField = BiotSavartField(wireCurrent, x1, y1, x2, y2);
double cosineTheta  = (x2 - x1)/ distance;
double bFieldX = cosineTheta * bField;
printf("B-Field X Component  = %g\n", bFieldX); 
return bFieldX;
}

std::vector<double> xValues(bool bPositive)
{
    std::vector<double> x; 
    for (int i=0; i<nWires; i++)
    {
        double theta = 2.* PI /(nWires)*i;
        if (!bPositive) theta += PI/(nWires); 
        x.push_back(radius*cos(theta));
        printf("positioning wire at x=%g\n", x[i]);
    }
    return x;
}
 
std::vector<double> yValues(bool bPositive)
{
    std::vector<double> y;
    for (int i=0; i<nWires; i++)
    {
        double theta = 2.* PI /(nWires)*i;
        if (!bPositive) theta += PI/(nWires);
	y.push_back(radius*sin(theta));
        printf("positioning wire at y=%g\n", y[i]);
    }
    return y;
}
    
double GetBFieldMag(double xPosition, double yPosition ) {
 
std::vector<double> xValPositive = xValues(true);
std::vector<double> yValPositive = yValues(true);

std::vector<double> xValNegative = xValues(false);
std::vector<double> yValNegative = yValues(false);

double bFieldXSum = 0.;
double bFieldYSum = 0.;   
    
for (int i=0; i<nWires; i++) {
        
bFieldXSum += bFieldX(400, xPosition, yPosition, xValPositive[i], yValPositive[i]);
bFieldYSum += bFieldY(400, xPosition, yPosition, xValPositive[i], yValPositive[i]);

bFieldXSum += bFieldX(-400, xPosition, yPosition, xValNegative[i], yValNegative[i]);
bFieldYSum += bFieldY(-400, xPosition, yPosition, xValNegative[i], yValNegative[i]);    
}

double bFieldMagSum = pow( bFieldXSum*bFieldXSum + bFieldYSum*bFieldYSum, 0.5);
printf("bFieldMagSum = %g\n", bFieldMagSum);
printf("bFieldXSum = %g\n", bFieldXSum);
printf("bFieldYSum = %g\n", bFieldYSum);
return bFieldMagSum;
}
   
TH2D* hFieldMap() { 

TH2D* hFieldMap = new TH2D("hMap", "Ioffe B-field Map; x (m) ; y (m) ", npixels, -0.3, 0.3, npixels, -0.3, 
0.3); 
hFieldMap->GetZaxis()->SetTitle("|B| (T)");
hFieldMap->GetZaxis()->SetTitleOffset(1.5);
hFieldMap->GetZaxis()->CenterTitle();

for (int i=0; i<npixels; i++) {
for (int j=0; j<npixels; j++) {

double xPosition = hFieldMap->GetXaxis()->GetBinCenter(i);
double yPosition = hFieldMap->GetYaxis()->GetBinCenter(j);
double fakeBfield = GetBFieldMag(xPosition, yPosition);
printf("fakeBfield is %g\n", fakeBfield);
// getchar();
hFieldMap->SetBinContent(i+1, j+1, fakeBfield);

}
}

return hFieldMap;
} 
void DrawCanvas(TGraph* aGraphPositive, TGraph* aGraphNegative, TH2D* hFieldMap)
{
    TCanvas* c1 = new TCanvas("Wire test", "Wire test", 700, 600);
    c1->cd();
    hFieldMap->DrawCopy("colz");
    aGraphPositive->SetMarkerSize(1.0);
    aGraphNegative->SetMarkerSize(1.0);    
    aGraphPositive->SetMarkerStyle(8);
    aGraphNegative->SetMarkerStyle(8);    
    aGraphPositive->SetMarkerColor(2);   
    aGraphNegative->SetMarkerColor(4);

    aGraphPositive->Draw("psame");
    aGraphNegative->Draw("psame");

    TImage *img = TImage::Create();
    img->FromPad(c1);
    img->WriteImage("./WireTest.png");
}
TGraph* gCircle(std::vector<double> xVal, std::vector<double> yVal)
{
    double x[nWires];
    double y[nWires];

    for (int i=0; i<nWires; i++)
    {
        x[i] = xVal[i];
        y[i] = yVal[i];
    }
    TGraph* gCirc = new TGraph(nWires, x, y);
    return gCirc;
} 
int go(){

gStyle->SetOptStat(0);
gStyle->SetPadRightMargin(0.18);
gStyle->SetPalette(kViridis);

std::vector<double> xValPositive = xValues(true);
std::vector<double> yValPositive = yValues(true);
std::vector<double> xValNegative = xValues(false);
std::vector<double> yValNegative = yValues(false);
TGraph* gWirePositionsPositive = gCircle(xValPositive, yValPositive);
TGraph* gWirePositionsNegative = gCircle(xValNegative, yValNegative);

TH2D* hMap = hFieldMap();
DrawCanvas(gWirePositionsPositive, gWirePositionsNegative,  hMap);
 
return 0;
}







