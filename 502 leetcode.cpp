struct Project {
    Project(int cap, int prof) {
        capital = cap;
        profit = prof;
        notRealized = true;
        index = LONG_MAX;
    }

    int capital;
    int profit;
    bool notRealized;
    int index;
};


// binary search approach
int FindLargestKeyLEQThanTarget(int target, std::vector<int>& keys) {
    if (!keys.size()) { return -2; }
    if (target < keys[keys.size() - 1]) { return -1; }

    int mid, left = 0, right = keys.size() - 1;
    while (left != right) {
        mid = (left + right) / 2;
        if (keys[mid] > target) { left = mid + 1; }
        else { right = mid; }
    }
    return keys[left];
}


int GetMinKeyInMap(std::vector<int>& keys) {
    if (keys.size()) { return keys[keys.size() - 1]; }
    return LONG_MAX;
}


static bool Comp(Project* pr1, Project* pr2) { 
    if (pr1->profit != pr2->profit) { return pr1->profit > pr2->profit; }
    return pr1->capital < pr2->capital;
}


int findMaximizedCapital(int k, int w, std::vector<int>& profits, std::vector<int>& capital) {
    std::vector<Project*> projects;
    for (int i = 0; i < profits.size(); ++i) {
        projects.push_back(new Project(capital[i], profits[i]));
    }
    std::sort(projects.begin(), projects.end(), Comp);
    for (int i = 0; i < projects.size(); ++i) {
        projects[i]->index = i;
    }

    std::unordered_map<int, Project*> bestNextProject;
    int keyNext, currentIndex = 0, amountOfProjects = projects.size();
    Project* current;
    std::vector<int> mapKeys_PriorityQueue;
    while (k > 0 && currentIndex < amountOfProjects) {
        current = projects[currentIndex];
        if (current->notRealized) {
            if (current->capital <= w) {
                current->notRealized = false;
                w += current->profit;
                --k;

                keyNext = FindLargestKeyLEQThanTarget(current->profit, mapKeys_PriorityQueue);
                if (keyNext == -2) {
                    currentIndex = 0;
                }
                else if (keyNext != -1) {
                    currentIndex = bestNextProject[keyNext]->index;
                }
                mapKeys_PriorityQueue.clear();
                bestNextProject.clear();
            }
            else {
                if (current->capital < GetMinKeyInMap(mapKeys_PriorityQueue)) {
                    bestNextProject[current->capital] = current;
                    mapKeys_PriorityQueue.push_back(current->capital);
                }
                ++currentIndex;
            }
        }
        else {
            ++currentIndex;
        }
    }

    return w;
}
