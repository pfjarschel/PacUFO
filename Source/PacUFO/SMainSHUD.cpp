//I was trying to make a Slate HUD, decided to use UMG later, ignore this. (I wanted to keep these files for possible future reference)

#include "PacUFO.h"
#include "SMainSHUD.h"
//#include "MainAHUD.h"
//#include "SlateOptMacros.h"
//
//#define LOCTEXT_NAMESPACE "SMainSHUD"
//
//BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
//void SMainSHUD::Construct(const FArguments& InArgs)
//{
//	OwnerHUD = InArgs._OwnerHUD;
//
//	ChildSlot
//	.VAlign(VAlign_Fill)
//	.HAlign(HAlign_Fill)
//	[
//		SNew(SOverlay)
//		+ SOverlay::Slot()
//		.VAlign(VAlign_Top)
//		.HAlign(HAlign_Center)
//		[
//			SNew(STextBlock)
//			.ShadowColorAndOpacity(FLinearColor::Black)
//		.ColorAndOpacity(FLinearColor::Red)
//		.ShadowOffset(FIntPoint(-1, 1))
//		.Font(FSlateFontInfo("Veranda", 16))
//		.Text(LOCTEXT("HelloSlate", "Hello, Slate!"))
//		]
//	];
//}
//END_SLATE_FUNCTION_BUILD_OPTIMIZATION
//
//#undef LOCTEXT_NAMESPACE