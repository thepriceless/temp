int longestArithSeqLength(vector<int>& nums) {
    std::vector<std::map<int, int>> maxLenforEachNum;
    maxLenforEachNum.resize(nums.size());
    int maxResult = 2;

    for (int i = 0; i < nums.size(); ++i) {
        for (int j = -500; j <= 500; ++j) {
            maxLenforEachNum[i][j] = 1;
        }
        for (int backI = 0; backI < i; ++backI) {
            int currentNum = maxLenforEachNum[backI][nums[backI] - nums[i]] + 1;
            maxResult = std::max(maxResult, currentNum);
            maxLenforEachNum[i][nums[backI] - nums[i]] = currentNum;
        }
    }

    return maxResult;
}
