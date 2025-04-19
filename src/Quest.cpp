// Quest.cpp
#include "../include/Quest.h"

Quest::Quest(const std::string& id, const std::string& name, const std::string& desc)
    : id(id), name(name), description(desc), status(QuestStatus::NOT_TAKEN) {}

std::string Quest::getId() const { return id; }
std::string Quest::getName() const { return name; }
std::string Quest::getDescription() const { return description; }
QuestStatus Quest::getStatus() const { return status; }

void Quest::take() { status = QuestStatus::TAKEN; }
void Quest::complete() { status = QuestStatus::COMPLETED; }
bool Quest::isCompleted() const { return status == QuestStatus::COMPLETED; }
