// QuestManager.h
#pragma once
#include <unordered_map>
#include <string>
#include "Quest.h"

class QuestManager {
private:
    std::unordered_map<std::string, Quest> quests;

public:
    void addQuest(const Quest& quest);
    bool hasQuest(const std::string& questId) const;
    QuestStatus getStatus(const std::string& questId) const;

    void takeQuest(const std::string& questId);
    void completeQuest(const std::string& questId);

    void showActiveQuests() const;
    void showCompletedQuests() const;
};
