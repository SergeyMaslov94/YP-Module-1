#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
	NEW,          // новая
	IN_PROGRESS,  // в разработке
	TESTING,      // на тестировании
	DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

TaskStatus Next(TaskStatus status)
{
	return static_cast<TaskStatus>(static_cast<int>(status) + 1);
} 

void RemoveZeros(TasksInfo& tasks) 
{
	for (TaskStatus status = TaskStatus::NEW; status != TaskStatus::DONE; status = Next(status)) {
		if (tasks[status] == 0)
			tasks.erase(status);
	}
}


class TeamTasks
{
private:


public:
	void SetInfo(std::string str, TasksInfo info) {
		tasks_for_person[str] = info;
	}

	map<string, TasksInfo> tasks_for_person;

	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const string& person) const
	{
		return tasks_for_person.at(person);
	}

	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const string& person)
	{
		tasks_for_person[person][TaskStatus::NEW] ++;
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count)
	{
		TasksInfo updated, untouched;

		// Если разработчика с именем person нет, метод PerformPersonTasks должен вернуть кортеж из двух пустых TasksInfo
		if (tasks_for_person.count(person) == 0)
		{
			return { updated, untouched };
		}

		TasksInfo& tmp_tasks = tasks_for_person[person];
		TasksInfo result_tasks;

		for (TaskStatus status = TaskStatus::NEW; status != TaskStatus::DONE; status = Next(status))
		{
			int to_update = std::min(tmp_tasks[status], task_count);
			task_count -= to_update;
			
			result_tasks[Next(status)] += updated[Next(status)] = to_update;
			result_tasks[status] += untouched[status] = tmp_tasks[status] - to_update;
		}

		result_tasks[TaskStatus::DONE] += tmp_tasks[TaskStatus::DONE];

		RemoveZeros(updated); 
		RemoveZeros(untouched);
		RemoveZeros(result_tasks);

		tmp_tasks = result_tasks;

		return { updated, untouched };
	}
};

//*************************************************************************************************

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь

void PrintTasksInfo(TasksInfo tasks_info) {
	cout << tasks_info[TaskStatus::NEW] << " new tasks"s <<
		", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s <<
		", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s <<
		", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

int run() {
	TeamTasks tasks;
	tasks.AddNewTask("Ilia");
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan");
	}
	cout << "Ilia's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
	cout << "Ivan's tasks: ";
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan", 2);
	cout << "Updated Ivan's tasks: ";
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: ";
	PrintTasksInfo(untouched_tasks);

	return 0;
}