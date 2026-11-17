#include "StudentMultiList.h"

int main() {
    StudentMultiList a;

    a.insertStudent(20220987, "G1", "Harley Kim");
    a.insertStudent(20231555, "G1", "William Jin");
    a.insertStudent(20221004, "G2", "Thomas Lee");
    a.insertStudent(19980123, "G1", "Jenny Anh");
    a.insertStudent(19980909, "G3", "Liam Son");
    a.insertStudent(19990131, "G2", "Owen Park");
    a.insertStudent(19981475, "-", "Aria Lee");
    a.insertStudent(20230554, "G2", "Oliver Kim");
    a.insertStudent(20240178, "G1", "Nora Kang");

    a.removeStudent(19980909);

    a.insertStudent(20250907, "G3", "Rosa Jang");

    cout << a.toStringForPrint() << endl;
    cout << "-------------------" << endl;
    cout << a.toStringForSave() << endl;
}