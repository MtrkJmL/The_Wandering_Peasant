// QuestManager.cpp
#include "QuestManager.h"
#include <iostream>

void QuestManager::addQuest(const Quest& quest) {
    quests[quest.getId()] = quest;
}

bool QuestManager::hasQuest(const std::string& questId) const {
    return quests.find(questId) != quests.end();
}

QuestStatus QuestManager::getStatus(const std::string& questId) const {
    auto it = quests.find(questId);
    if (it != quests.end()) {
        return it->second.getStatus();
    }
    return QuestStatus::NOT_TAKEN;
}

void QuestManager::takeQuest(const std::string& questId) {
    auto it = quests.find(questId);
    if (it != quests.end()) {
        it->second.take();
        std::cout << "\nQuest taken: " << it->second.getName() << "\n\n";
    }
}

void QuestManager::completeQuest(const std::string& questId) {
    auto it = quests.find(questId);
    if (it != quests.end()) {
        it->second.complete();
        std::cout << "\nQuest completed: " << it->second.getName() << "\n\n";
    }
}

void QuestManager::showActiveQuests() const {
    
    for (const auto& [id, quest] : quests) {
        if (quest.getStatus() == QuestStatus::TAKEN) {
            std::cout << quest.getName() << ": " << quest.getDescription() << "\n";
        }
    }
}

void QuestManager::showCompletedQuests() const {
    
    for (const auto& [id, quest] : quests) {
        if (quest.isCompleted()) {
            std::cout << quest.getName() << " [✓]\n";
        }
    }
}
