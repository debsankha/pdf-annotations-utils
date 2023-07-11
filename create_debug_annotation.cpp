#include <iostream>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <QColor>



#include <poppler-qt5.h>
#include <QFont>

using namespace Poppler;
using namespace std;

bool annotations_eq(Annotation* annot, Annotation* annot2) {
    return (annot->creationDate() == annot2->creationDate())
    & (annot->boundary() == annot2->boundary());
}

// look at okular src: generators/poppler/annots.cpp::265

int main() {
    // we will add stuff from file1 to file2
    QString file1 = "/Users/debsankha.manik/private_projects/poppler-qt5/small_plain.pdf";
    QString file2 = "/Users/debsankha.manik/private_projects/poppler-qt5/small_manual.pdf";
    Document* doc = Document::load(file1);

    // now read the pages
    Page* page = doc->page(0);

    // sanity check: Creating a text annotation works
    auto text = new TextAnnotation(Poppler::TextAnnotation::TextType::InPlace);
    text->setContents("hello world");
    text->setBoundary({0.5, 0.5, 0.2,0.1});
    page->addAnnotation(text);

    // sanity check: Geom
    auto gannot = new GeomAnnotation();
    gannot->setGeomType(GeomAnnotation::GeomType::InscribedCircle);
    gannot->setBoundary({0.3, 0.3, 0.2, 0.2});
    gannot->setGeomInnerColor({255,0,0,255});
//    gannot->setContents("whfbciluqefbncl/an");
//    gannot->style().setWidth(10.0);
//    gannot->style().setColor(QColorConstants::DarkGreen);
//    gannot->style().setOpacity(1.0);

    page->addAnnotation(gannot);

    //creating a new InkAnnotation does not work
//    auto newannot = new InkAnnotation();
//
//    newannot->setBoundary({0.9, 0.9, 0.8, 0.8});
//    newannot->setContents({"def"});
//    newannot->setUniqueName({"gth"});
//
//    newannot->setFlags(0);
//
//    auto s = newannot->style();
//    s.setColor({0,0,0,255});
//    s.setLineStyle(Annotation::LineStyle::Solid);
//
//    s.setWidth(2);
//    s.setOpacity(1);
//    s.setLineEffect(Annotation::LineEffect::NoEffect);
//
//
//    QList<QLinkedList<QPointF>> inkPaths{
//        {
//                {0.1,0.1}, {0.2, 0.2}, {0.3, 0.3}, {0.9, 0.9},
//        }
//    };
//    newannot->setInkPaths(inkPaths);
//    newannot->setStyle(s);
//    newannot->setBoundary({0.1, 0.1, 0.8, 0.8});
//
//
//    page->addAnnotation(newannot);

    // write pdf
    PDFConverter *pdfConv = doc->pdfConverter();
    pdfConv->setOutputFileName(file2);
    pdfConv->setPDFOptions(pdfConv->pdfOptions()|PDFConverter::WithChanges);
    bool success = pdfConv->convert();
    delete pdfConv;

    return 0;
}