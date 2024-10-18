#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iomanip> 
using namespace std;

//С������
struct Community {
    string name;       // С������
    int buildingCount; // ¥������
    vector<int> buildingNumbers; // ¥�����б�
};
//��������
struct HouseType {
    string id;           // ���ͱ��
    float area;          // �������
    float fee;            // �շѱ�׼
};
//ס������
struct Resident {
    string id;                // ס�����
    string houseId;        // ס�����ͱ��
    string name;           // ס������
    string phone;          // ��ϵ�绰
    string address;        // ��ϵ��ַ
    string password;       // ס������
    float prepaidAmount;   //Ӧ�����
    float unpaidAmount;    // Ƿ�ѽ��
};
//ס�����޹�����Ͷ�߹���
struct Complaint {
    string residentId;        // Ͷ��ס�����
    string description;    // Ͷ������
    bool resolved;         // �Ƿ��ѽ��
};
struct Repair {
    string residentId;        // ����ס�����
    string description;    // ��������
    bool resolved;         // �Ƿ��ѽ��
};
//ס����ҵ�շѹ���
struct PropertyFee {
    int id;
    string name;
    float amount;          // �շѽ��
    string unit;           // �շѵ�λ����Ԫ/�£�Ԫ/����Ԫ/�꣩
    string period;         // �շ�����
};

struct Community communities;
vector<HouseType> houseTypes;
vector<Resident> residents;     //ס������
vector<Complaint> complaints;
vector<Repair> repairs;
vector<PropertyFee> propertyfees;

vector<Resident> unpaidResidents;//δ����ҵ��ס���б�

string adminPassword = "admin123"; // ����Ա����

void adminMenu(void);
void userMenu(int residentId);
bool Login(string role);

void addResident(void);
void modifyResident(void);
void viewResident(void);
void deleteResident(void);

void loadfromFile(const string& filename);
void savetoFile(const string &filename);

void modifyCommunity(void);
void viewCommunity(void);

void addHousetype(void);
void modifiyHousetype(void);
void deleteHousetype(void);
void viewHousetype(void);
void countHousetype(void);

void printRepair(Repair repair);
void printComplaint(Complaint complaint);
void printHouseType(HouseType houseType);
void printResident(Resident resident);

void addComplaints(string role, string userid);
void deleteComplaints(void);
void modifiyComplaints(void);
void viewComplaints(void);

void addRepairs(string role, string userid);
void modifiyRepairs(void);
void deleteRepairs(void);
void viewRepairs(void);

void findUnpaidResident(void);
void sortUnpaid();
void remindUser(string userid);

void printFee(PropertyFee fee);
void viewFee();
void deleteFee();
void modifiyFee();
void addFee();


//���ļ���ȡ
void loadfromFile(const string& filename) {
    vector<string> fileList = {
        "./data/residents.txt",
        "./data/community.txt",
        "./data/housetype.txt",
        "./data/complaints.txt",
        "./data/repairs.txt",
        "./data/fees.txt"
    };
    //ס�� ��С�� ������ ��Ͷ�ߡ� ���ޡ�����
    int fileindex;
    if (filename == "resident") fileindex = 0;
    else if (filename == "community") fileindex = 1;
    else if (filename == "housetype") fileindex = 2;
    else if (filename == "complaint") fileindex = 3;
    else if (filename == "repair") fileindex = 4;
    else if (filename == "fee") fileindex = 5;
    else fileindex = -1;
    
    if (fileindex != -1) {
        ifstream file(fileList[fileindex]);//���ļ�
        if (!file) {            //����ļ��Ƿ�ɹ���
            cout << "�޷����ļ�:" << fileList[fileindex] << endl;
            return;
        }
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            switch (fileindex)
            {
            case 0:
            {
                Resident resident;
                // ����һ�����ݣ������ļ��ж��ŷָ��ĸ�ʽ
                string prepaidAmount, unpaidAmount;
                getline(ss, resident.id, ',');               // ��ȡס�����
                getline(ss, resident.name, ',');    // ��ȡס������
                getline(ss, resident.phone, ',');   // ��ȡ��ϵ�绰
                getline(ss, resident.address, ','); // ��ȡ��ϵ��ַ
                getline(ss, prepaidAmount, ',');    // ��ȡӦ�����
                getline(ss, unpaidAmount, ',');     // ��ȡǷ�ѽ��
                getline(ss, resident.houseId, ',');          // ��ȡס������
                getline(ss, resident.password, ',');
                resident.prepaidAmount = stof(prepaidAmount);
                resident.unpaidAmount = stof(unpaidAmount);
                residents.push_back(resident);
                break;
            }
            case 1: // ��ȡС����Ϣ        
            {
                string buildingCount;
                getline(ss, communities.name, ',');
                getline(ss, buildingCount, ',');
                communities.buildingCount = stoi(buildingCount);
                break;
            }
            case 2: // ��ȡ������Ϣ       
            {
                HouseType houseType;
                string area, fee;
                getline(ss, houseType.id, ',');
                getline(ss, area, ',');
                getline(ss, fee, ',');
                houseType.area = stof(area);
                houseType.fee = stof(fee);
                houseTypes.push_back(houseType);
                break;
            }
            case 3: // ��ȡͶ����Ϣ
            {
                Complaint complaint;
                string isresolved;
                getline(ss, complaint.residentId, ',');
                getline(ss, complaint.description, ',');
                getline(ss, isresolved, ',');
                complaint.resolved = (isresolved == "true") ? true : false;
                complaints.push_back(complaint);
                break;
            }
            case 4: // ��ȡ������Ϣ
            {
                Repair repair;
                string isresolved;
                getline(ss, repair.residentId, ',');
                getline(ss, repair.description, ',');
                getline(ss, isresolved, ',');
                repair.resolved = (isresolved == "true") ? true : false;
                repairs.push_back(repair);
                break;
            }
            case 5: // ��ȡ������Ϣ
            {
                PropertyFee fee;
                string id, amount;
                getline(ss, id, ',');
                getline(ss, fee.name, ',');
                getline(ss, amount, ',');
                getline(ss, fee.unit, ',');
                getline(ss, fee.period, ',');
                fee.id = stoi(id);
                fee.amount = stof(amount);
                propertyfees.push_back(fee);
                //cout << "���ü���" << endl;

                break;
            }
            default:break;
            }
        }
        cout << "�ļ� " << fileList[fileindex] << " ���سɹ���" << endl;
        file.close();
    }
}
//���浽�ļ�
void savetoFile(const string &filename) {
    vector<string> fileList = {
        "./data/residents.txt",
        "./data/community.txt",
        "./data/housetype.txt",
        "./data/complaints.txt",
        "./data/repairs.txt",
        "./data/fees.txt"
    };
    int fileindex;
    if (filename == "resident") fileindex = 0;
    else if (filename == "community") fileindex = 1;
    else if (filename == "housetype") fileindex = 2;
    else if (filename == "complaint") fileindex = 3;
    else if (filename == "repair") fileindex = 4;
    else if (filename == "fee") fileindex = 5;
    else fileindex = -1;

    if (fileindex != -1) {
        ofstream file(fileList[fileindex]);  // ios::app ��������׷��
        if (!file) {  // ����ļ��Ƿ�ɹ���
            cout << "�޷����ļ�" << endl;
            return;
        }

        switch (fileindex) {
        case 0:  // residents
        {
            for (const auto& resident : residents) {
                file << resident.id << ","
                    << resident.name << ","
                    << resident.phone << ","
                    << resident.address << ","
                    << resident.prepaidAmount << ","
                    << resident.unpaidAmount << ","
                    << resident.houseId<<","
                    << resident.password <<endl;
            }
            break;  // ����� break ��������Ϊ�� switch ��һ����
        }
        case 1:  // communities
        {
            file << communities.name << ","
                << communities.buildingCount << endl;
            break;
        }
        case 2:  // houseTypes
        {
            for (const auto& housetype : houseTypes) {
                file << housetype.id << ","
                    << housetype.area << ","
                    << housetype.fee << endl;
            }
            break;
        }
        case 3:  // complaints
        {
            for (const auto& complaint : complaints) {
                file << complaint.residentId << ","
                    << complaint.description << ","
                    << complaint.resolved << endl;
            }
            break;
        }
        case 4:  // repairs
        {
            for (const auto& repair : repairs) {
                file << repair.residentId << ","
                    << repair.description << ","
                    << repair.resolved << endl;
            }
            break;
        }
        case 5:
        {
            for (const auto& fee : propertyfees) {
                file << fee.id << ","
                    << fee.name << ","
                    << fee.amount << ","
                    << fee.unit << ","
                    << fee.period << endl;
            }
            break;
        }
        default: break;
        }

        file.close();  // �ر��ļ�
        cout << "�ļ��ѱ���" << endl;
    }
}


// ��ӡס����Ϣ
void printResident(Resident resident) {
    cout << setw(10) << resident.id << "\t";
    cout << setw(5) << resident.name << "\t";
    cout << setw(15) << resident.phone << "\t";
    cout << setw(10) << resident.address << "\t";
    cout << setw(3) << resident.houseId << "\t";
    cout << setw(8) << resident.prepaidAmount << "\t";
    cout << setw(8) << resident.unpaidAmount << "\t";
    cout << setw(8) << resident.password << endl;
}

// ��ӡ������Ϣ
void printHouseType(HouseType houseType) {
    cout << setw(3) << houseType.id << "\t";
    cout << setw(5) << houseType.area << "\t";
    cout << setw(5) << houseType.fee << endl;

}

// ��ӡͶ����Ϣ
void printComplaint(Complaint complaint) {
    cout << setw(10) << complaint.residentId << "\t";
    cout << setw(20) << complaint.description << "\t";
    cout << setw(4) << (complaint.resolved ? "��" : "��") << endl;

}

// ��ӡ������Ϣ
void printRepair(Repair repair) {
    cout << setw(10) << repair.residentId << "\t";
    cout << setw(20) << repair.description << "\t";
    cout << setw(4) << (repair.resolved ? "��" : "��") << endl;
}


void printFee(PropertyFee fee) {
    cout << setw(3) << fee.id << "\t";
    cout << setw(10) << fee.name << "\t";
    cout << setw(5) << fee.amount << "\t";
    cout << setw(4) << fee.unit << "\t";
    cout << setw(4) << fee.period << endl;
}

//����ס��
void findResident() {
    string id;
    cout << "����Ҫ���ҵ�ס��id:" << endl;
    cin >> id;

    for (const auto& resident : residents) {
        if (resident.id == id) {
            cout << setw(10) << "\033[33mס�����" << "\t";
            cout << setw(5) << "����" << "\t";
            cout << setw(15) << "�绰" << "\t";
            cout << setw(10) << "סַ" << "\t";
            cout << setw(3) << "����" << "\t";
            cout << setw(8) << "�������" << "\t";
            cout << setw(8) << "δ�����" << "\t";
            cout << setw(8) << "�û�����\033[0m" << endl;
            printResident(resident);
            system("pause");
            return;
        }
    }
    cout << "id���������ס�������ڣ�" << endl;
    system("pause");

}

//����ס��
void addResident() {
    Resident resident;
    cout << "\033c";
    cout << "********����ס��********:" << endl;
    cout << "����ס�����:" << endl;
    cin >> resident.id;
    cout << "����ס������:" << endl;
    cin >> resident.name;
    cout << "������ϵ�绰:" << endl;
    cin >> resident.phone;
    cout << "������ϵ��ַ:" << endl;
    cin >> resident.address;
    cout << "����ס������:" << endl;
    cin >> resident.houseId;
    cout << "����ס������:" << endl;
    cin >> resident.password;
    resident.prepaidAmount = 0;
    resident.unpaidAmount = 0;
    //������ס����ӵ���ס����
    residents.push_back(resident);
    cout << "ס����ӳɹ���" << endl;
    savetoFile("resident");
    system("pause");
}

//�޸�ס��
void modifyResident() {
    string id;
    cout << "����Ҫ�޸ĵ�ס��id:" << endl;
    cin >> id;

    for (auto& resident : residents) {
        if (resident.id == id) {
            cout << "����ס������:" << endl;
            cin >> resident.name;
            cout << "������ϵ�绰:" << endl;
            cin >> resident.phone;
            cout << "������ϵ��ַ:" << endl;
            cin >> resident.address;
            cout << "����ס������:" << endl;
            cin >> resident.houseId;
            cout << "����ס������:" << endl;
            cin >> resident.password;
            savetoFile("resident");
            cout << "�޸ĳɹ���" << endl;

            system("pause");

            return;
        }
    }
    cout << "id���������ס�������ڣ�" << endl;
    system("pause");

}

//�鿴ס��
void viewResident() {
    if (residents.size() == 0) {
        cout<<"ס���б�Ϊ�գ�" << endl;
    }
    else {
        cout << setw(10) << "\033[33mס�����" << "\t";
        cout << setw(5) << "����" << "\t";
        cout << setw(15) << "�绰" << "\t";
        cout << setw(10) << "סַ" << "\t";
        cout << setw(3) << "����" << "\t";
        cout << setw(8) << "�������" << "\t"; 
        cout << setw(8) << "δ�����" << "\t";
        cout << setw(8) << "�û�����\033[0m" << endl;
        for (const auto& resident : residents) {
            printResident(resident);
        }
        
    }
    
    system("pause");
}

//ɾ��ס��
void deleteResident() {
    string id;
    cout << "����Ҫɾ����ס��id:" << endl;
    cin >> id;

    int cont = 0;
    for (const auto& resident : residents) {
        if (resident.id == id) {
            cout << "���ҵ�ס����" << endl;
            cout << setw(10) << "\033[33mס�����" << "\t";
            cout << setw(5) << "����" << "\t";
            cout << setw(15) << "�绰" << "\t";
            cout << setw(10) << "סַ" << "\t";
            cout << setw(3) << "����" << "\t";
            cout << setw(8) << "�������" << "\t";
            cout << setw(8) << "δ�����" << "\t";
            cout << setw(8) << "�û�����\033[0m" << endl;
            printResident(resident);
            cout << "�Ƿ�ɾ����ס����(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                residents.erase(residents.begin() + cont);
                savetoFile("resident");

            }
            else {
                cout << "ȡ������" << endl;
            }
            system("pause");
            return;
        }
        cont++;
    }
    cout << "id���������ס�������ڣ�" << endl;
    system("pause");

}

//�޸�С������
void modifyCommunity() {
    cout << "\033c";
    cout << "����С������:" << endl;
    cin >> communities.name;
    cout << "����¥������:" << endl;
    cin >> communities.buildingCount;
    communities.buildingNumbers.clear();
    for (int i = 1; i < communities.buildingCount + 1; i++) {
        communities.buildingNumbers.push_back(i);
    }
    savetoFile("community");

    system("pause");

}
//�鿴С������
void viewCommunity() {
    cout << "\033c";
    cout <<"С�����ƣ�" << communities.name << endl;
    cout <<"¥��������" << communities.buildingCount << endl;
    system("pause");

}

//��ӷ���
void addHousetype() {
    HouseType houseType;
    cout << "********����ס��********:" << endl;
    cout << "���뷿�ͱ��:" << endl;
    cin >> houseType.id;
    cout << "���뷿�����:" << endl;
    cin >> houseType.area;
    cout << "���뷿���շѱ�׼:" << endl;
    cin >> houseType.fee;
    //������ס����ӵ���ס����
    houseTypes.push_back(houseType);
    savetoFile("housetype");
    cout << "������ӳɹ���" << endl;
    system("pause");

}

//�޸ķ���
void modifiyHousetype() {
    string id;
    cout << "����Ҫ�޸ĵķ��ͱ��:" << endl;
    cin >> id;
    for (auto& housetype : houseTypes) {
        if (housetype.id == id) {
            cout << "�޸ķ������:" << endl;
            cin >> housetype.area;
            cout << "�޸ķ����շѱ�׼:" << endl;
            cin >> housetype.fee;
            savetoFile("housetype");
            cout << "�������޸ģ�" << endl;
            system("pause");

            return;
        }
    }
    cout << "����������Ͳ����ڣ�" << endl;
    system("pause");
}

//ɾ������
void deleteHousetype() {
    string id;
    cout << "����Ҫɾ���ķ��ͱ��:" << endl;
    cin >> id;

    int cont = 0;
    for (const auto& housetype : houseTypes) {
        if (housetype.id == id) {
            cout << "���ҵ����ͣ�" << endl;
            cout << setw(3) << "\033[33m���" << "\t";
            cout << setw(5) << "���" << "\t";
            cout << setw(5) << "�շѱ�׼\033[0m" << endl;
            printHouseType(housetype);
            cout << "\n\n�Ƿ�ɾ���÷��ͣ�(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                houseTypes.erase(houseTypes.begin() + cont);
                savetoFile("housetype");
            }
            else {
                cout << "ȡ������" << endl;
            }
            return;
        }
        cont++;
    }
    cout << "����������Ͳ����ڣ�" << endl;
    system("pause");
}

//�鿴���з���
void viewHousetype() {
    if (houseTypes.size() == 0) {
        cout << "�����б�Ϊ��" << endl;
    }
    else {
        cout << setw(3) << "\033[33m���" << "\t";
        cout << setw(5) << "���" << "\t";
        cout << setw(5) << "�շѱ�׼\033[0m" << endl;
        for (const auto& housetype : houseTypes) {
            printHouseType(housetype);
        }
    }

    system("pause");
}

//���ҷ���
void findHousetype() {
    string id;
    cout << "����Ҫ���ҵķ��ͱ��:" << endl;
    cin >> id;

    for (const auto& housetype : houseTypes) {
        if (housetype.id == id) {
            cout << setw(3) << "\033[33m���" << "\t";
            cout << setw(5) << "���" << "\t";
            cout << setw(5) << "�շѱ�׼\033[0m" << endl;
            printHouseType(housetype);
            system("pause");
            return;
        }
    }
    cout << "����������Ͳ����ڣ�" << endl;
    system("pause");

}

//����ͳ��
void countHousetype() {
    struct Counthouse {
        string id;         //����id
        int count = 0;      //��������
    };
    vector<Counthouse> counthouse(houseTypes.size()); //���ݷ��͸�������������С
    int i = 0;
    for (const auto& housetype : houseTypes) {          //���������б���ÿ������id�����½ṹ��counthouse������ͳ��
        counthouse[i].id = housetype.id;
        i++;
    }
    for (const auto& resident : residents) {
        for (auto& count : counthouse) {
            if (resident.houseId == count.id) {
                count.count++;
            }
        }
    }
    //��ʾͳ�ƽ��
    for (auto& count : counthouse) {
        cout << "���ͣ�" << count.id << " ����" << count.count <<" ��" << endl;
    }
    system("pause");
}

//Ͷ���½�
void addComplaints(string role, string userid) {
    Complaint complaint;
    if (role == "admin") {
        cout << "\033c";
        cout << "********����Ͷ����Ϣ********:" << endl;
        cout << "����Ͷ��ס�����:" << endl;
        cin >> complaint.residentId;
        cout << "����Ͷ������:" << endl;
        cin >> complaint.description;
        char option;
        cout << "�Ƿ��ѽ��(Y/N):" << endl;
        cin >> option;
        if (option == 'Y') {
            complaint.resolved = true;
        }
        else if (option == 'N') {
            complaint.resolved = false;
        }
        else {
            cout << "��������" << endl;

        }
        //��Ͷ����Ϣ��ӵ���Ͷ����Ϣ��               
    }
    else if (role == "user") {
        cout << "\033c";
        cout << "********����Ͷ����Ϣ********:" << endl;
        complaint.residentId = userid;
        cout << "����Ͷ������:" << endl;
        cin >> complaint.description;
        complaint.resolved = false;
    }
    complaints.push_back(complaint);
    savetoFile("complaint");
    cout << "Ͷ����Ϣ��ӳɹ���" << endl;
}
//Ͷ���޸�
void modifiyComplaints() {
    string id;
    cout << "����Ҫ�޸ĵ�Ͷ��ס�����:" << endl;
    cin >> id;

    for (auto& complaint : complaints) {
        if (complaint.residentId == id) {
            cout << "�޸�Ͷ������:" << endl;
            cin >> complaint.description;
            char option;
            cout << "�Ƿ��ѽ��(Y/N):" << endl;
            cin >> option;
            if (option == 'Y') {
                complaint.resolved = true;
            }
            else if (option == 'N') {
                complaint.resolved = false;
            }
            else {
                cout << "��������" << endl;
            }
            savetoFile("complaint");

            system("pause");
            return;
        }
    }
    cout << "����������Ͳ����ڣ�" << endl;
    system("pause");
}

//Ͷ��ɾ��
void deleteComplaints() {
    string id;
    cout << "����Ҫɾ����Ͷ��ס�����:" << endl;
    cin >> id;

    int cont = 0;
    for (auto& complaint : complaints) {
        if (complaint.residentId == id) {
            cout << "���ҵ�ס����" << endl;
            cout << "ס����ţ�" << complaint.residentId << endl;
            cout << "�Ƿ�ɾ����ס��Ͷ����Ϣ��(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                complaints.erase(complaints.begin() + cont);
                savetoFile("complaint");

            }
            else {
                cout << "ȡ������" << endl;
            }
            system("pause");
            return;
        }
        cont++;
    }
    cout << "��������" << endl;
    system("pause");
}

//�鿴Ͷ��
void viewComplaints() {
    if (complaints.size() == 0) {
        cout << "Ͷ���б�Ϊ��" << endl;
    }
    else {
        cout << setw(10) << "\033[33mͶ�߱��" << "\t";
        cout << setw(20) << "Ͷ������" << "\t";
        cout << setw(4) << "�Ƿ���\033[0m" << endl;
        for (const auto& complaint : complaints) {
            printComplaint(complaint);
        }
    }

    system("pause");
}

//����Ͷ��
void findComplaints() {
    string id;
    cout << "����Ҫ���ҵ�Ͷ��ס�����:" << endl;
    cin >> id;

    for (auto& complaint : complaints) {
        if (complaint.residentId == id) {
            cout << setw(10) << "\033[33mͶ�߱��" << "\t";
            cout << setw(20) << "Ͷ������" << "\t";
            cout << setw(4) << "�Ƿ���\033[0m" << endl;
            printComplaint(complaint);
            system("pause");
            return;
        }
    }
    cout << "����������Ͳ����ڣ�" << endl;
    system("pause");
}

//�����½�
void addRepairs(string role, string userid) {
    Repair repair;
    if (role == "admin") {
        cout << "\033c";
        cout << "********����������Ϣ********:" << endl;
        cout << "���뱨��ס�����:" << endl;
        cin >> repair.residentId;
        cout << "���뱨������:" << endl;
        cin >> repair.description;
        char option;
        cout << "�Ƿ��ѽ��(Y/N):" << endl;
        cin >> option;
        if (option == 'Y') {
            repair.resolved = true;
        }
        else if (option == 'N') {
            repair.resolved = false;
        }
        else {
            cout << "��������" << endl;

        }
    }
    else if (role == "admin") {
        repair.residentId = userid;
        cout << "���뱨������:" << endl;
        cin >> repair.description;
        repair.resolved = false;
    }
    //��������Ϣ��ӵ��ܱ�����Ϣ��
    repairs.push_back(repair);
    savetoFile("repair");

    cout << "������Ϣ��ӳɹ���" << endl;
    system("pause");
}

//�����޸�
void modifiyRepairs() {
    string id;
    cout << "����Ҫ�޸ĵı���ס�����:" << endl;
    cin >> id;

    for (auto& repair : repairs) {
        if (repair.residentId == id) {
            cout << "�޸ı�������:" << endl;
            cin >> repair.description;
            char option;
            cout << "�Ƿ��ѽ��(Y/N):" << endl;
            cin >> option;
            if (option == 'Y') {
                repair.resolved = true;
            }
            else if (option == 'N') {
                repair.resolved = false;
            }
            else {
                cout << "��������" << endl;
            }
            savetoFile("repair");

            system("pause");
            return;
        }
    }
    cout << "����������Ͳ����ڣ�" << endl;
    system("pause");
}

//����ɾ��
void deleteRepairs() {
    string id;
    cout << "����Ҫɾ���ı���ס�����:" << endl;
    cin >> id;

    int cont = 0;
    for (auto& repair : repairs) {
        if (repair.residentId == id) {
            cout << "���ҵ�ס����" << endl;
            cout << "ס����ţ�" << repair.residentId << endl;
            cout << "�Ƿ�ɾ����ס��������Ϣ��(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                repairs.erase(repairs.begin() + cont);
                savetoFile("repair");

            }
            else {
                cout << "ȡ������" << endl;
            }
            system("pause");
            return;
        }
        cont++;
    }
    cout << "��������" << endl;
    system("pause");
}

//�鿴����
void viewRepairs() {
    if (repairs.size() == 0) {
        cout << "Ͷ���б�Ϊ��" << endl;
    }
    else {
        cout << setw(10) << "\033[33m���ޱ��" << "\t";
        cout << setw(20) << "��������" << "\t";
        cout << setw(4) << "�Ƿ���\033[0m" << endl;
        for (const auto& repair : repairs) {
            printRepair(repair);
        }
    }
    system("pause");
}

//���ұ���
void findRepairs() {
    string id;
    cout << "����Ҫ���ҵı���ס�����:" << endl;
    cin >> id;

    for (auto& repair : repairs) {
        if (repair.residentId == id) {
            cout << setw(10) << "\033[33m���ޱ��" << "\t";
            cout << setw(20) << "��������" << "\t";
            cout << setw(4) << "�Ƿ���\033[0m" << endl;
            printRepair(repair);
            system("pause");
            return;
        }
    }
    cout << "����������Ͳ����ڣ�" << endl;
    system("pause");
}



//����Ƿ��ס��
void findUnpaidResident() {
    unpaidResidents.clear();
    for (const auto& resident : residents) {
        if (resident.unpaidAmount > 0) {

            unpaidResidents.push_back(resident);
        }
    }
    sortUnpaid();
    cout << "Ƿ��ס��������Ƿ��������" << endl;
    cout << setw(10) << "\033[33mס�����" << "\t";
    cout << setw(5) << "����" << "\t";
    cout << setw(15) << "�绰" << "\t";
    cout << setw(10) << "סַ" << "\t";
    cout << setw(8) << "�������" << "\t";
    cout << setw(8) << "δ�����\033[0m" << endl;
    for (const auto& resident : unpaidResidents) {
        cout << setw(10) << resident.id << "\t";
        cout << setw(5) << resident.name << "\t";
        cout << setw(15) << resident.phone << "\t";
        cout << setw(10) << resident.address << "\t";
        cout << setw(8) << resident.prepaidAmount << "\t";
        cout << setw(8) << resident.unpaidAmount << endl;
    }
    system("pause");
}


//Ƿ��ס������
void sortUnpaid() {
    
    sort(unpaidResidents.begin(), unpaidResidents.end(), [](const Resident& a, const Resident& b) {
        return a.unpaidAmount > b.unpaidAmount; });  // Ƿ������ס������ǰ��
}

//������ҵ��
void calcFee() {
    
    for (auto& resident : residents) {
        float feeAmount = 0.0;
        for (const auto& housetype : houseTypes) {
            if (housetype.id == resident.houseId) {
                feeAmount = float(housetype.area * housetype.fee);
            }
        }
        for (const auto& fee : propertyfees) {
            feeAmount += fee.amount;
        }
        resident.prepaidAmount = feeAmount;
    }
    cout << "��ҵ���Ѽ�����ɡ�" << endl;
    savetoFile("resident");
    system("pause");
}



void addFee() {
    PropertyFee fee;
    cout << "********�����շ�********:" << endl;
    cout << "�����շѱ��:" << endl;
    cin >> fee.id;
    cout << "�����շ�����:" << endl;
    cin >> fee.name;
    cout << "�����շѽ��:" << endl;
    cin >> fee.amount;
    cout << "�����շѵ�λ:" << endl;
    cin >> fee.unit;
    cout << "�����շ�����:" << endl;
    cin >> fee.period;
    propertyfees.push_back(fee);
    savetoFile("fee");
    cout << "�շ���ӳɹ���" << endl;
    system("pause");

}

//�޸�fee
void modifiyFee() {
    int id;
    cout << "����Ҫ�޸ĵ��շѱ��:" << endl;
    cin >> id;
    for (auto& fee : propertyfees) {
        if (fee.id == id) {
            cout << "�����շѱ��:" << endl;
            cin >> fee.id;
            cout << "�����շ�����:" << endl;
            cin >> fee.name;
            cout << "�����շѽ��:" << endl;
            cin >> fee.amount;
            cout << "�����շѵ�λ:" << endl;
            cin >> fee.unit;
            cout << "�����շ�����:" << endl;
            cin >> fee.period;
            savetoFile("fee");
            cout << "�շ����޸ģ�" << endl;
            system("pause");

            return;
        }
    }
    cout << "�����������ò����ڣ�" << endl;
    system("pause");
}

//ɾ��fee
void deleteFee() {
    int id;
    cout << "����Ҫɾ�����շѱ��:" << endl;
    cin >> id;

    int cont = 0;
    for (const auto& fee : propertyfees) {
        if (fee.id == id) {
            cout << "���ҵ��շѣ�" << endl;
            printFee(fee);
            cout << "\n\n�Ƿ�ɾ�����շѣ�(Y/N)" << endl;
            char option;
            cin >> option;
            if (option == 'Y') {
                propertyfees.erase(propertyfees.begin() + cont);
                savetoFile("fee");
            }
            else {
                cout << "ȡ������" << endl;
            }
            return;
        }
        cont++;
    }
    cout << "�����������ò����ڣ�" << endl;
    system("pause");
}

//�鿴�շ�
void viewFee() {
    if (propertyfees.size() == 0) 
    {
        cout << "�����б�Ϊ��" << endl;
    }
    else 
    {
        cout << setw(3) << "\033[33m���" << "\t";
        cout << setw(10) << "��������" << "\t";
        cout << setw(5) << "���" << "\t";
        cout << setw(4) << "��λ" << "\t";
        cout << setw(4) << "�ɷ�����\033[0m" << endl;
        for (const auto& fee : propertyfees) {
            printFee(fee);
        }
    }
    system("pause");
}




//��������
void changePassWord(string resi) {
    for (auto& resident : residents) {
        if (resident.id == resi) {
            cout << "����������:" << endl;
            cin >> resident.password;
        }
    }
    savetoFile("resident");
    system("pause");
}
//����Ա�˵�
void adminMenu() {
    int choice;
    do {
        cout << "\033c";
        cout << "\033[33m*******************\033[5m  ��ӭʹ��С����ҵ����ϵͳ��\033[0m\033[33m*********************\033[0m" << endl;
        cout << "                                                                  " << endl;
        cout << "                           ����Ա�˵�                               " << endl;
        cout << "                                                                  " << endl;
        cout << "                           1.С������                                 "<< endl;
        cout << "                           2.���͹���                                 " << endl;
        cout << "                           3.ס������                                 " << endl;
        cout << "                           4.���޹���                             " << endl;
        cout << "                           5.Ͷ�߹���                              " << endl;
        cout << "                           6.��ҵ�շѹ���                          " << endl;
        cout << "                           0:�˳�                                     " << endl;
        cout << "                                                                  " << endl;
        cout << "\033[33m********************************************************************\033[0m" << endl;
        cout << "\033[32m��ѡ�����\033[0m��" << endl;
        cin >> choice;
        int choice_next = 0;
        do
        {
            switch (choice)
            {
            case 1:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  ��ӭʹ��С����ҵ����ϵͳ��\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      ����Ա�˵�>С������                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                       1.�޸�����                                  " << endl;
                cout << "                       2.�鿴����                                  " << endl;
                cout << "                       0:�˳�                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m��ѡ�����\033[0m��" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:modifyCommunity(); break;
                case 2:viewCommunity(); break;
                case 0:break;
                default:
                    cout << "��Чѡ��!" << endl;
                    break;
                }
                break;
            case 2:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  ��ӭʹ��С����ҵ����ϵͳ��\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      ����Ա�˵�>���͹���                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.��������                                  " << endl;
                cout << "                        2.�޸ķ���                                  " << endl;
                cout << "                        3.ɾ������                                  " << endl;
                cout << "                        4.�鿴����                                  " << endl;
                cout << "                        5.����ͳ��                                  " << endl;
                cout << "                        6.���ҷ���                                  " << endl;
                cout << "                        0:�˳�                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m��ѡ�����\033[0m��" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:addHousetype(); break;
                case 2:modifiyHousetype(); break;
                case 3:deleteHousetype(); break;
                case 4:viewHousetype(); break;
                case 5:countHousetype(); break;
                case 6:findHousetype(); break;

                case 0:break;
                default:
                    cout << "��Чѡ��!" << endl;
                    break;
                }
                break;
            case 3:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  ��ӭʹ��С����ҵ����ϵͳ��\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      ����Ա�˵�>ס������                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.����ס��                                  " << endl;
                cout << "                        2.�޸�ס��                                  " << endl;
                cout << "                        3.ɾ��ס��                                  " << endl;
                cout << "                        4.�鿴ס��                                  " << endl;
                cout << "                        5.����ס��                                  " << endl;
                cout << "                        0:�˳�                                     " << endl;
                cout << "                                                                   " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m��ѡ�����\033[0m��" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:addResident(); break;
                case 2:modifyResident(); break;
                case 3:deleteResident(); break;
                case 4:viewResident(); break;
                case 5:findResident(); break;

                case 0:break;
                default:
                    cout << "��Чѡ��!" << endl;
                    break;
                }
                break;
            case 4:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  ��ӭʹ��С����ҵ����ϵͳ��\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      ����Ա�˵�>���޹���                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.��������                                  " << endl;
                cout << "                        2.�޸ı���                                  " << endl;
                cout << "                        3.ɾ������                                  " << endl;
                cout << "                        4.�鿴����                                  " << endl;
                cout << "                        5.���ұ���                                  " << endl;
                cout << "                        0:�˳�                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m��ѡ�����\033[0m��" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:addRepairs("admin", "0"); break;
                case 2:modifiyRepairs(); break;
                case 3:deleteRepairs(); break;
                case 4:viewRepairs(); break;
                case 5:findRepairs(); break;
                case 0:break;
                default:
                    cout << "��Чѡ��!" << endl;
                    break;
                }
                break;
            case 5:
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  ��ӭʹ��С����ҵ����ϵͳ��\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                      ����Ա�˵�>Ͷ�߹���                           " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.����Ͷ��                                  " << endl;
                cout << "                        2.�޸�Ͷ��                                  " << endl;
                cout << "                        3.ɾ��Ͷ��                                  " << endl;
                cout << "                        4.�鿴Ͷ��                                  " << endl;
                cout << "                        5.����Ͷ��                                  " << endl;
                cout << "                        0:�˳�                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "\033[32m��ѡ�����\033[0m��" << endl;
                cin >> choice_next;
                switch (choice_next)
                {
                case 1:addComplaints("admin", "0"); break;
                case 2:modifiyComplaints(); break;
                case 3:deleteComplaints(); break;
                case 4:viewComplaints(); break;
                case 5:findComplaints(); break;
                case 0:break;
                default:
                    cout << "��Чѡ��!" << endl;
                    break;
                }
                break;
            case 6:
            {
                cout << "\033c";
                cout << "\033[33m*******************\033[5m  ��ӭʹ��С����ҵ����ϵͳ��\033[0m\033[33m*********************\033[0m" << endl;
                cout << "                                                                  " << endl;
                cout << "                  ����Ա�˵�>ס����ҵ�շѹ���                       " << endl;
                cout << "                                                                  " << endl;
                cout << "                        1.����δ�ɷ�ס��                            " << endl;
                cout << "                        2.����ס����ҵ��                                " << endl;
                cout << "                        3.����δ�ɷ�ס��                            " << endl;
                cout << "                        4.�鿴�շ�                            " << endl;
                cout << "                        5.�޸��շ�                            " << endl;
                cout << "                        6.ɾ���շ�                            " << endl;
                cout << "                        7.�����շ�                            " << endl;
                cout << "                        0:�˳�                                     " << endl;
                cout << "                                                                  " << endl;
                cout << "\033[33m********************************************************************\033[0m" << endl;
                cout << "��ѡ�������" << endl;

                cin >> choice_next;
                switch (choice_next)
                {
                case 1:findUnpaidResident(); break;
                case 2:calcFee();break;
                case 3:
                {
                    cout << "�����ѷ��͵�ס������";
                    system("pause");
                    break;
                }
                case 4:viewFee(); break;
                case 5:modifiyFee(); break;
                case 6:deleteFee(); break;
                case 7:addFee(); break;
                case 0:break;
                default:
                    break;
                }
            }
            default:

                break;
            }
            } while (choice_next != 0);
        } while (choice != 0);
    
}

//ס���˵�
void userMenu(string residentId) {
    int choice;
    do {
        cout << "\033c";
        remindUser(residentId);
        cout << "\033[33m*******************\033[5m  ��ӭʹ��С����ҵ����ϵͳ��\033[0m\033[33m*********************\033[0m" << endl;
        cout << "                                                                  " << endl;
        cout << "                             ס���˵�                                " << endl;
        cout << "                                                                  " << endl;
        cout << "                             1.�ύͶ��                                  " << endl;
        cout << "                             2.�鿴Ͷ��                                 " << endl;
        cout << "                             3.�ύ����                                  " << endl; 
        cout << "                             4.�鿴����                                  " << endl;
        cout << "                             5.�޸�����                                  " << endl;
        cout << "                             0:�˳�                                     " << endl;
        cout << "                                                                  " << endl;
        cout << "\033[33m********************************************************************\033[0m" << endl;
        cout << "��ѡ�������";

        cin >> choice;

        switch (choice)
        {
        case 1:addComplaints("user", residentId);
            break;
        case 2:
        {
            for (const auto& complaint : complaints) {
                if (complaint.residentId == residentId) {
                    cout << setw(10) << "\033[33mͶ�߱��" << "\t";
                    cout << setw(20) << "Ͷ������" << "\t";
                    cout << setw(4) << "�Ƿ���\033[0m" << endl;
                    printComplaint(complaint);
                }
            }
            system("pause");
        }
            break;
        case 3:addRepairs("user", residentId);
            break;
        case 4:
        {
            for (const auto& repair : repairs) {
                if (repair.residentId == residentId) {
                    cout << setw(10) << "\033[33m���ޱ��" << "\t";
                    cout << setw(20) << "��������" << "\t";
                    cout << setw(4) << "�Ƿ���\033[0m" << endl;
                    printRepair(repair);
                }
            }
            system("pause");
           
        }
        break;
        case 5:
            changePassWord(residentId);
            break;
        
        default:
            break;
        }
    } while (choice != 0);
}
//����ס���ɷ�
void remindUser(string userid) {
    for (const auto& resident : residents) {
        if (resident.id == userid && resident.unpaidAmount > 0) {
            cout << "\n\n" << "������δ�ɵ���ҵ�� " << resident.unpaidAmount << "Ԫ��\n" << endl;
        }
    }
}


bool Login(string role, string userid) {
    string inputPassword;
    int wrongtime=0;
    while (wrongtime < 3) {
        if (role == "admin") {
            cout << "���������Ա����:" << endl;
            cin >> inputPassword;
            if (inputPassword == adminPassword) {
                return true;
            }
            else {
                cout << "��¼ʧ�ܣ��������" << endl;
                wrongtime++;
            }

        }
        else if (role == "user") {

            for (const auto& resident : residents) {

                if (resident.id == userid) {
                    // �ҵ�ס������ʼ��֤����
                    cout << "������ס��" << userid << "������:" << endl;
                    cin >> inputPassword;
                    if (inputPassword == resident.password) {
                        cout << "��¼�ɹ���" << endl;
                        return true;

                    }
                    else {
                        cout << "��¼ʧ�ܣ��������" << endl;
                        wrongtime++;
                        
                    }
                }


            }
            cout << "û�и�ס����" << endl;
            system("pause");
            return false;
            
        }
    }
    return false;
}


//������
int main() {
    
    loadfromFile("resident");
    loadfromFile("community");
    loadfromFile("housetype");
    loadfromFile("complaint");
    loadfromFile("repair");
    loadfromFile("fee");
    int option = 0;
    do {
        cout << "\033c";

        cout << "\033[33m===============С����ҵ����ϵͳ��¼================\033[0m\n" << endl;
        cout << "\033[32m                 ��ѡ����ݣ�" << endl;
        cout << "\033[32m                  1. ����Ա " << endl;
        cout << "\033[32m                  2. ס��\n" << endl;
        cout << "\033[33m===================================================\033[0m" << endl;


        cin >> option;
        if (option == 1 && Login("admin", "0")) {

            adminMenu();
        }
        else if (option == 2) {
            string userid;
            cout << "������ס��id" << endl;
            cin >> userid;
            if (Login("user", userid)) {
                userMenu(userid);
            }
            else {
                continue;
            }
        }
        else {
            cout << "���˳���" << endl;
        }
    } while (option != 0);


    system("pause");
    return 0;
}
