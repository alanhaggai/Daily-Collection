QT += sql
TEMPLATE = app
TARGET = 
DEPENDPATH += . forms include res src
INCLUDEPATH += include

OBJECTS_DIR = .obj
UI_DIR = .ui
MOC_DIR = .moc
RCC_DIR = .res

# Input
HEADERS += include/CreateAgentDialog.h \
           include/CreateDebtorDialog.h \
           include/DailyCollectionWindow.h \
           include/DaybookAgentDialog.h \
           include/DaybookAllAgentsDialog.h \
           include/DaybookBalanceDialog.h \
           include/DbConnect.h \
           include/DebtorDetailsDialog.h \
           include/DebtorInstallmentDialog.h \
           include/DebtorTransactionsDialog.h \
           include/EditAgentDialog.h \
           include/EditDebtorDialog.h \
           include/EditTransactionDialog.h \
           include/SettingsDialog.h \
           include/TransactionsDialog.h
FORMS += forms/CreateAgentDialog.ui \
         forms/CreateDebtorDialog.ui \
         forms/DailyCollectionWindow.ui \
         forms/DaybookAgentDialog.ui \
         forms/DaybookAllAgentsDialog.ui \
         forms/DaybookBalanceDialog.ui \
         forms/DebtorDetailsDialog.ui \
         forms/DebtorInstallmentDialog.ui \
         forms/DebtorTransactionsDialog.ui \
         forms/EditAgentDialog.ui \
         forms/EditDebtorDialog.ui \
         forms/EditTransactionDialog.ui \
         forms/SettingsDialog.ui \
         forms/TransactionsDialog.ui
SOURCES += src/CreateAgentDialog.cpp \
           src/CreateDebtorDialog.cpp \
           src/DailyCollectionWindow.cpp \
           src/DaybookAgentDialog.cpp \
           src/DaybookAllAgentsDialog.cpp \
           src/DaybookBalanceDialog.cpp \
           src/DbConnect.cpp \
           src/DebtorDetailsDialog.cpp \
           src/DebtorInstallmentDialog.cpp \
           src/DebtorTransactionsDialog.cpp \
           src/EditAgentDialog.cpp \
           src/EditDebtorDialog.cpp \
           src/EditTransactionDialog.cpp \
           src/main.cpp \
           src/SettingsDialog.cpp \
           src/TransactionsDialog.cpp
RESOURCES += res/dailycollection.qrc
