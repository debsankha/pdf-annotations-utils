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
    QString file1 = "/Users/debsankha.manik/private_projects/poppler-qt5/small_merged.pdf";
    QString file2 = "/Users/debsankha.manik/private_projects/poppler-qt5/small_kobo.pdf";
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

        cout << (oldannot1->style() == oldannot2->style()) << endl;
    }
    return 0;
}