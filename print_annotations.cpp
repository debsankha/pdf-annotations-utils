#include <iostream>
#include <format>
#include <iostream>
#include <string>
#include <string_view>


#include <poppler-qt5.h>
#include <QFont>

using namespace Poppler;
using namespace std;

bool annotations_eq(Annotation* annot, Annotation* annot2) {
    return (annot->creationDate() == annot2->creationDate())
    & (annot->boundary() == annot2->boundary());
}


int main() {
    // we will add stuff from file1 to file2
    QString file1 = "/Users/debsankha.manik/private_projects/poppler-qt5/small_kobo.pdf";
    QString file2 = "/Users/debsankha.manik/private_projects/poppler-qt5/small_merged.pdf";
    QString file3 = "/Users/debsankha.manik/private_projects/poppler-qt5/small_xxx.pdf";
    Document* doc1 = Document::load(file1);
    Document* doc2 = Document::load(file2);

    // now read the pages
    for (auto num_page=0; num_page <1; num_page++)
    {
        Page* page1 = doc1->page(num_page);
        Page* page2 = doc2->page(num_page);

        auto annot1 = page1->annotations()[0];
        auto annot2 = page2->annotations()[0];

        std::cout<<"page: " << num_page << ", type: ink" << std::endl;
        auto oldannot1 = dynamic_cast<InkAnnotation *>(annot1);
        auto oldannot2 = dynamic_cast<InkAnnotation *>(annot2);

        // check that styles are equal
        auto s1 = oldannot1->style();
        auto s2 = oldannot2->style();
        assert(s1.color()==s2.color());
        assert(s1.opacity()==s2.opacity());
        assert(s1.lineStyle()==s2.lineStyle());
        assert(s1.effectIntensity()==s2.effectIntensity());
        assert(s1.dashArray()==s2.dashArray());
        assert(s1.lineEffect()==s2.lineEffect());
        assert(s1.width()==s2.width());

        // check other stuff
        assert(oldannot1->flags() == oldannot2->flags());
        assert(oldannot1->contents() == oldannot2->contents());
        s2.setOpacity(0.5);
        s2.setColor({145,184,168, 10});
        cout<<"width: "<<s2.width()<<endl;
        s2.setWidth(4);

        //create a new annotation object
        auto newannot = new InkAnnotation();

        // add other properties
        //newannot->setFlags(oldannot2->flags());
        //newannot->setContents(oldannot2->contents());

        newannot->setAnnotationAppearance(*oldannot2->annotationAppearance());
        newannot->setInkPaths(oldannot2->inkPaths());
        //newannot->setBoundary(oldannot2->boundary());
        newannot->setStyle(s2);

        auto bound = oldannot2->boundary();
        cout<<format("Boundary: x {}, y {}, w {}, h {}\n", bound.x(), bound.y(), bound.width(), bound.height());

        auto inkpaths = oldannot2->inkPaths();

        auto path_num=0;
        for (auto& inkpath: inkpaths)
        {
            cout<<"path_num: "<<path_num<<endl;
            path_num++;
            cout <<"[";
            for (auto& p: inkpath)
            {
                cout<<format("{{{:.3f},{:.3f}}}, ",p.x(),p.y());
            }
            cout <<"]\n";
        }
        // print contents
        cout<<"contents: "<<oldannot2->contents().toStdString()<<endl;

        // print flags
        cout<<"flags: "<<oldannot2->flags()<<endl;
        cout<<"opacity: "<<oldannot2->style().opacity()<<endl;

        auto c = oldannot2->style().color();
        cout<<format("color: ({}, {}, {}, {})", c.red(), c.green(), c.blue(), c.alpha())<<endl;

        cout<<"linestyle "<<oldannot2->style().lineStyle()<<endl;
        cout<<"lineffect "<<oldannot2->style().lineEffect()<<endl;
        cout<<"effectintensity: "<<oldannot2->style().effectIntensity()<<endl;

        // pen properties
        cout << format("xcorners: {}, xcorners:  {}", s2.xCorners(), s2.yCorners());
        //const QVector<double> &dashArray() const; // [ 3 ]
        //void setDashArray(const QVector<double> &array);















        // add to page
        page2->removeAnnotation(oldannot2);
        page2->addAnnotation(newannot);

    }

    PDFConverter *pdfConv = doc2->pdfConverter();
    pdfConv->setOutputFileName(file3);
    pdfConv->setPDFOptions(pdfConv->pdfOptions()|PDFConverter::WithChanges);
    bool success = pdfConv->convert();
    delete pdfConv;
    return 0;

    return 0;
}