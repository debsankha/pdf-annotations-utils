#include <iostream>
#include <format>
#include <poppler-qt5.h>
#include <QFont>
#include <QCommandLineParser>


using namespace Poppler;

bool annotations_eq(Annotation* annot1, Annotation* annot2) {
    return (annot1->creationDate() == annot2->creationDate())
    & (annot1->boundary() == annot2->boundary());
}


int main(int argc, char*argv[]) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("my-copy-program");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("copies PDF annotations from one file to another");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file1", QCoreApplication::translate("main", "PDF file to copy annotations from"));
    parser.addPositionalArgument("file2", QCoreApplication::translate("main", "PDF file to copy annotations to"));
    parser.addPositionalArgument("outfile", QCoreApplication::translate("main", "new PDF file"));

    // Process the actual command line arguments given by the user
    parser.process(app);
    const QStringList args = parser.positionalArguments();
    auto file1 = args.at(0);
    auto file2 = args.at(1);
    auto fileout = args.at(2);

    // we will add stuff from file1 to file2
    Document* doc1 = Document::load(file1);
    Document* doc2 = Document::load(file2);

    // now read the pages
    for (auto num_page=0; num_page <doc1->numPages(); num_page++)
    {
        Page* page1 = doc1->page(num_page);
        Page* page2 = doc2->page(num_page);

        for (auto annot1: page1->annotations())
        {
            bool is_in_2 = false;
            for (auto annot2: page2->annotations())
            {
                // heuristic to determine if two annotations are the same
                if (annotations_eq(annot1, annot2)) is_in_2=true;
            }
            if (!is_in_2)
            {
                Annotation* newannot_base;
                bool tocopy=false;
                switch (annot1->subType()) {
                    case Annotation::AHighlight: {
                        tocopy = true;
                        auto oldannot = dynamic_cast<HighlightAnnotation *>(annot1);
                        auto newannot = new HighlightAnnotation();
                        newannot_base = static_cast<Annotation *>(newannot);
                        // set stuff
                        newannot->setHighlightQuads(oldannot->highlightQuads());
                        newannot->setHighlightType(oldannot->highlightType());
                        //newannot.setAnnotationAppearance(oldannot->annotationAppearance());
                        break;
                    }
                    case Annotation::AText: {
                        tocopy = true;
                        auto oldannot = dynamic_cast<TextAnnotation *>(annot1);
                        auto newannot = new TextAnnotation(oldannot->textType());
                        newannot_base = static_cast<Annotation *>(newannot);
                        // set stuff
                        newannot->setInplaceAlign(oldannot->inplaceAlign());
                        newannot->setInplaceIntent(oldannot->inplaceIntent());
                        newannot->setTextColor(oldannot->textColor());
                        newannot->setTextFont(oldannot->textFont());
                        newannot->setTextIcon(oldannot->textIcon());
                        break;
                    }
                    case Annotation::AInk: {
                        tocopy = true;
                        auto oldannot = dynamic_cast<InkAnnotation *>(annot1);
                        auto newannot = new InkAnnotation();
                        newannot_base = static_cast<Annotation *>(newannot);
                        // set stuff
                        // setting inkpath not easy
                        newannot->setInkPaths(oldannot->inkPaths());
                        break;
                    }
                    case Annotation::AGeom: {
                        tocopy = true;
                        auto oldannot = dynamic_cast<GeomAnnotation *>(annot1);
                        auto newannot = new GeomAnnotation();
                        newannot_base = static_cast<Annotation *>(newannot);
                        // set stuff
                        newannot->setGeomType(oldannot->geomType());
                        newannot->setGeomInnerColor(oldannot->geomInnerColor());
                        break;
                    }
                    case Annotation::ALine: {
                        tocopy = true;
                        auto oldannot = dynamic_cast<LineAnnotation*>(annot1);
                        auto newannot = new LineAnnotation(oldannot->lineType());
                        newannot_base = static_cast<Annotation *>(newannot);
                        // set stuff
                        newannot->setLineClosed(oldannot->isLineClosed());
                        newannot->setLineStartStyle(oldannot->lineStartStyle());
                        newannot->setLineEndStyle(oldannot->lineEndStyle());
                        newannot->setLineInnerColor(oldannot->lineInnerColor());
                        newannot->setLineIntent(oldannot->lineIntent());
                        newannot->setLineLeadingBackPoint(oldannot->lineLeadingBackPoint());
                        newannot->setLineLeadingForwardPoint(oldannot->lineLeadingBackPoint());
                        newannot->setLinePoints(oldannot->linePoints());
                        newannot->setLineShowCaption(oldannot->lineShowCaption());
                        break;
                    }
                    case Annotation::AStamp:
                        break;
                    case Annotation::ALink:
                        break;
                    case Annotation::ACaret:
                        break;
                    case Annotation::AFileAttachment:
                        break;
                    case Annotation::ASound:
                        break;
                    case Annotation::AMovie:
                        break;
                    case Annotation::AScreen:
                        break;
                    case Annotation::AWidget:
                        break;
                    case Annotation::ARichMedia:
                        break;
                    case Annotation::A_BASE:
                        break;
                }
                if (tocopy) {
                    // appearance related stuff
                    newannot_base->setStyle(annot1->style());
                    newannot_base->setAnnotationAppearance(*annot1->annotationAppearance());
                    // non appearance stuff
                    newannot_base->setBoundary(annot1->boundary());
                    newannot_base->setContents(annot1->contents());
                    newannot_base->setCreationDate(annot1->creationDate());
                    newannot_base->setUniqueName(annot1->uniqueName());
                    newannot_base->setAuthor(annot1->author());
                    newannot_base->setFlags(annot1->flags());
                    newannot_base->setPopup(annot1->popup());

                    page2->addAnnotation(newannot_base);
                }
            }
        }
    }

    PDFConverter *pdfConv = doc2->pdfConverter();
    pdfConv->setOutputFileName(fileout);
    std::cout <<std::format("writing output: {}\n", fileout.toStdString());
    pdfConv->setPDFOptions(pdfConv->pdfOptions()|PDFConverter::WithChanges);
    bool success = pdfConv->convert();
    delete pdfConv;
    return 0;
}
