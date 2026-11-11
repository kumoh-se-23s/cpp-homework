/*
Student: id, group, name을 가지고 있는 데이터클래스
-getId, getGroup, getName
-toString

StudentNode: student, *nextTeammate, *prevTeammate, *nextClassmate를 가지는 Node
-getStudent, getNextTeammate, getPrevTeammate, getNextClassmate
-setNextTeammate, setPrevTeammate, setNextClassmate
-toString

StudentSinglyList: *nextClassmate만을 관리하는 자료구조
-findById(int id) (return StudentNode*)
-addNewStudent(StudentNode*) : 오름차순 삽입
-removeStudent(StudentNode*)

StudentCircularDoublyList: *nextTeammate와 *prevTeammate만을 관리하는 자료구조
-addNewStudent(StudentNode*) : 오름차순 삽입
-removeStudent(StudentNode*)

IdMap: int key(입학년도)와 StudentSinglyList를 가지는 맵
-findById(int id) (return StudentNode*)
-getKey, getList

GroupMap: MySTring key(그룹명)와 StudentCircularDoublyList를 가지는 맵
-getKey, getList

IdTable: IdMap들의 ArrayList
-getListById(int id)
-addNewStudent(StudentNode*)
-removeStudent(StudentNode*)

GroupTable: GroupMap들의 ArrayList
-getListByGroup(MyString group)
-addNewStudent(StudentNode*)
-removeStudent(StudentNode*)

StudentManager: StudentNode의 생성,삭제를 전담, IdTable과 GroupTable을 가지고 모든 자료구조 동작을 관리
-printAllStudent
-addNewStudent(int id, MyString group, MyString name)
-removeStudent(int id)

ManageCircleApp: StudentManager를 가진 동작부 app
-runApp

*/