// Quest.h
#pragma once
#include <string>
#include "QuestStatus.h"

class Quest {
private:
    std::string id;
    std::string name;
    std::string description;
    QuestStatus status;

public:
    Quest() : id(""), name(""), description(""), status(QuestStatus::NOT_TAKEN) {}
    Quest(const std::string& id, const std::string& name, const std::string& desc);

    std::string getId() const;
    std::string getName() const;
    std::string getDescription() const;
    QuestStatus getStatus() const;

    void take();
    void complete();
    bool isCompleted() const;
};
