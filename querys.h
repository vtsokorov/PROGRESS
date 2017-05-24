#ifndef QUERYS
#define QUERYS

#include <QString>

namespace Querys {

const QString lastSelling =
    "select selling.selling_date, employee.employee_name, equipment.equipment_name, "
        "equipment.equipment_price, selling.selling_amount, "
        "selling.selling_sum, paytype.definition "
    "from selling "
       "inner join paytype on (selling.id_paytype = paytype.id_paytype) "
       "inner join employee on (selling.id_employee = employee.id_employee) "
       "inner join equipment on (selling.id_equipment = equipment.id_equipment) "
       "where selling.selling_date = (select max(selling.selling_date) from selling)";

const QString daySelling =
    "select selling.selling_date, employee.employee_name, equipment.equipment_name, "
             "equipment.equipment_price, selling.selling_amount, selling.selling_sum, "
             "paytype.definition "
    "from selling "
       "inner join employee on (selling.id_employee = employee.id_employee) "
       "inner join paytype on (selling.id_paytype = paytype.id_paytype)"
       "inner join equipment on (selling.id_equipment = equipment.id_equipment) "
    "where selling.selling_date = '%0' ";

const QString dayDelivery =
    "select equipment.delivery_date, equipment.equipment_name, equipment.equipment_amount, "
        "equipment.equipment_price, provider.provider_name, provider.provider_address, "
        "provider.provider_telephone "
    "from equipment "
       "inner join provider on (equipment.id_provider = provider.id_provider) "
    "where equipment.delivery_date = '%0'";

const QString sumPeriod =
        "select paytype.definition, sum( selling.selling_sum ) sum_of_selling_sum, "
            "count( paytype.definition ) count_of_definition "
        "from equipment "
           "inner join selling on (equipment.id_equipment = selling.id_equipment) "
           "inner join paytype on (selling.id_paytype = paytype.id_paytype) "
        "where ((selling.selling_date >= '%0' and selling.selling_date <= '%1')) "
        "group by paytype.definition";

const QString goodsInStock =
        "select equipment.equipment_name, equipment.equipment_amount,"
               "equipment.equipment_price "
        "from equipment "
          "where equipment.equipment_amount > 0";

const QString employeeList =
    "select employee.id_employee, employee.employee_name, "
        "employee.employee_position, employee.employee_telephone "
    "from employee";

const QString priceList =
        "select equipment.id_equipment, equipment.equipment_name, "
            "equipment.equipment_price, equipment.equipment_guarantee "
        "from equipment";

const QString providersList =
        "select  provider.id_provider, provider.provider_name, "
            "provider.provider_address, provider.provider_telephone "
        "from provider";

const QString countSelling =
        "select equipment.equipment_name, employee.employee_name, "
            "selling.selling_amount, selling.selling_sum, paytype.definition "
        "from selling "
           "inner join employee on (selling.id_employee = employee.id_employee) "
           "inner join paytype on (selling.id_paytype = paytype.id_paytype) "
           "inner join equipment on (selling.id_equipment = equipment.id_equipment) "
        "where selling.selling_date >= '%0' and selling.selling_date <= '%1'";

const QString deliveryPeriod =
        "select equipment.delivery_date, provider.provider_name, "
            "provider.provider_address, equipment.equipment_name, "
            "equipment.equipment_price, equipment.equipment_amount "
        "from provider "
           "inner join equipment on (provider.id_provider = equipment.id_provider) "
        "where ((equipment.delivery_date >= '%0') and (equipment.delivery_date <= '%1'))";

}

/*
Отчеты:

Прайс-лист
Список поставщиков
Список сотрудников

*/

#endif // QUERYS



