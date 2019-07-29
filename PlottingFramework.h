#ifndef PlottingFramework_h
#define PlottingFramework_h

#include "TFile.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "THn.h"
#include "TIterator.h"
#include "TKey.h"
#include "TObjArray.h"
#include "TObject.h"
#include "TSystem.h"
#include <TROOT.h>
#include <TObjectTable.h>
#include "TExec.h"
#include "TTimeStamp.h"
#include "TLatex.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TText.h"
#include "TLatex.h"
#include "TError.h"
#include "TSpline.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TPaletteAxis.h"
#include <iostream>
#include "Plot.h"

namespace PlottingProject {
  using std::cout;
  using std::endl;
  using std::flush;
  using std::string;
  using std::vector;
  
  
  /** \class PlottingFramework
   This is a generic plotting framework for managing your histograms and plotting from different sources using ROOT functionality.
   */
  class PlottingFramework
  {
  public:
    struct GlobalStyle{
      int pixelBaseLength;
      int fillStyleCanvas;
      double aspectRatio; // for asymmetric plots
      vector<int> defaultMarkers;
      vector<int> defaultMarkersFull;
      vector<int> defaultMarkersOpen;
      vector<int> defaultColors;
      bool useCMYK;
      int palette;
      bool drawTimestamps;
      vector<double> timestampPosition;
      int textFont;
      int lableFont;
      int titleFont;
      
      double markerSize;
      double markerSizeThick;
      
      double lineWidth;
      double lineWidthThick;
      
      double textSize;
      double lableSizeScaling;
      double titleSizeScaling;
    };
    
    typedef struct PadStyle{
      vector<double> position;  // xlow, ylow, xup, yup (relative to canvas)
      vector<double> margin; // top, bottom, left, right
      vector<double> titleOffset; // x, y, z
    } PadStyle;
    
    typedef struct CanvasStyle{
      string styleName;
      double canvasWidth;
      double canvasHeight;
      vector<PadStyle> pads;
    } CanvasStyle;
    
    PlottingFramework();
    virtual ~PlottingFramework();
    
    TPaveText* MakeText(Plot::TextBox& textBoxTemplate);
    TLegend* MakeLegend(TPad* pad, Plot::TextBox& legendBoxTemplate, vector<Plot::Histogram>& histos);
    
    int GetDefaultColor(int colorIndex);
    int GetDefaultMarker(int markerIndex);
    
    void SetPalette(int palette);
    void AddHistosFromInputFile(string datasetIdentifier, string inputFileName);
    void CreateDatasetPlots(string datasetIdentifier);
    void CreateTemporaryPlots(string datasetIdentifier){}
    void SetOutputDirectory(string path);
    void SavePlot(string plotName, string figureGroup, string subFolder = "");
    void WritePlotsToFile(string outputFileName); //todo: also file structure
    void ListPlots(){mPlotLedger->ls();}
    void ListHistos(){mHistoLedger->ls();}
    
    // load all or one specific plot defined in conf file
    void LoadAvailablePlots(string configFileName){}
    // TODO gibts schon...
    void SavePlotTemplate(string plotName, string configFileName, bool replace = false){}
    void LoadPlotTemplate(string plotName, string configFileName){}
    void CreateNewPlot(Plot &plot, string canvasStyleName, bool saveToConfig = false);
    void AddPlotToConfig(Plot &myPlot){}
    void RemovePlotFromConfig(string plotName){}
    
    void AdjustLableOffsets();
    
    TGraphErrors* GetGraphClone(string graphName);
    TH1* GetHistClone(string histName);
    TH1* GetRatio(string ratioName);
    
    void SetTextSize(int size){}
    void SetPixelBase(int size = 710){mStyle.pixelBaseLength = size;}
    
    void SetAxes(TPad* pad, Plot &plot);
    void SetTransparentCanvas(bool transparent = true);
    
    void ListAvailableCanvasStyles();
    
    void SetDrawTimestamps(bool drawTimestamps = true) {mStyle.drawTimestamps = drawTimestamps;}
    
    bool ContainsDatasets(std::initializer_list<string> requiredDatasets);
    void CutHistogram(TH1* hist, double cutoff, double cutoffLow);
    
  private:
    GlobalStyle mStyle;
    void DefineDefaultPlottingStyle();
    void SetStyle(TCanvas* canvas);
    
    vector<CanvasStyle> mCanvStyles;
    CanvasStyle* GetCanvasStyle(string styleName);
    
    TCanvas* MakeCanvas(string name, CanvasStyle& canvasStyle);
    void ApplyStyleSettings(CanvasStyle& canvasStyle, TCanvas* canvas, string controlString);
    
    TObjArray* mHistoLedger;   ///<!  Container to store input histograms
    TObjArray* mPlotLedger;    ///<!  Container to store output plots
    string mOutputDirectory;  ///<!  Directory to store output
    // actually vector<Plot>
    
    vector<string> mDatasetIdentifiers;
    
    // Bookkeeping functions
    void BookHistos(TDirectoryFile& folder, string datasetIdentifier); // recursively walking through input file
    //   void RemoveHisto(string histName);
    
    bool IsPlotPossible(Plot &plot);
    
    void CreatePlot(string name, string datasetIdentifier);
    void SetGlobalStyle();
    void DrawPlotInCanvas(Plot &plot, TCanvas* canvas);
    
    // Helper functions
    // string GetDatasetSuffix(TH1* hist, string datasetIdentifier);
    TH1* DivideWithTSpline(TH1* numerator, TH1* denominator);
    void ApplyHistoSettings(TH1* histo, Plot::Histogram &histoTemplate, string &drawingOptions, int defaultValueIndex, string controlString);
    
    PlottingFramework(const PlottingFramework&) = default;
    PlottingFramework& operator=(const PlottingFramework&) = default;
  };
}
#endif
