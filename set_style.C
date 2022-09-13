
void style(TStyle* gStyle) {

	gStyle->SetOptStat("");
	gStyle->SetOptFit(0000);
	gStyle->SetTitleBorderSize(0);
	gStyle->SetTitleFontSize(0.03);
	gStyle->SetTitleX(0.5);
	gStyle->SetTitleY(0.97);
	gStyle->SetTitleAlign(23);
	gStyle->SetStatY(0.85);
	gStyle->SetStatX(0.48);
	gStyle->SetStatW(0.17);
	gStyle->SetStatH(0.08);
	gStyle->SetPadLeftMargin(0.15);
   	gStyle->SetPadRightMargin(0.15);
   	gStyle->SetPadTopMargin(0.08);
	gStyle->SetPadBottomMargin(0.125);
        gStyle->SetLegendBorderSize(0);
        gStyle->SetLegendTextSize(0.03);
	gStyle->SetPadBorderSize(2);

}

void histo_style(TH1D* h) {

	h->SetTitle("");
    	// x axis
   	h->GetXaxis()->SetLabelSize(0.03);
   	h->GetXaxis()->SetTitleSize(0.03);
   	h->GetXaxis()->SetTitleOffset(1);
   	h->GetXaxis()->SetTickLength(0.02);
    	h->GetXaxis()->SetMaxDigits(3);

    	// y axis
   	h->GetYaxis()->SetLabelSize(0.03);
   	h->GetYaxis()->SetTitleSize(0.03);
   	h->GetYaxis()->SetTitleOffset(1.35);   
   	h->GetYaxis()->SetTickLength(0.02); 
	h->GetYaxis()->SetMaxDigits(3);

    	// fill and line
    	//h->SetFillStyle(3001); 
	Int_t ci; // color index setting
	ci = TColor::GetColor("#004C97");
	h->SetFillColorAlpha(ci,0.2);
	h->SetLineColor(ci);
}

void graph_style(TGraph* g) {

	g->SetTitle("");
    	// x axis
   	g->GetXaxis()->SetLabelSize(0.035);
   	g->GetXaxis()->SetTitleSize(0.035);
   	g->GetXaxis()->SetTitleOffset(1);
   	g->GetXaxis()->SetTickLength(0.02);
   	 g->GetXaxis()->SetMaxDigits(3);

   	// y axis
   	g->GetYaxis()->SetLabelSize(0.035);
   	g->GetYaxis()->SetTitleSize(0.035);
   	g->GetYaxis()->SetTitleOffset(1.5);    
   	g->GetYaxis()->SetTickLength(0.02);
   	g->GetYaxis()->SetMaxDigits(3);

}




