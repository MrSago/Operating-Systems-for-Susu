
#include "processscheduler.h"

ProcessScheduler::ProcessScheduler(QAbstractItemModel* table_model,
                                   QObject* parent)
    : model(table_model), QObject{parent} {}
