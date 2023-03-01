void QuickSort(std::vector<int>& nums, int Ileft, int Iright) {
    if (Ileft >= Iright) { return; }

    int rememberLeft = Ileft, rememberRight = Iright--;
    int pivotElement = nums[(rememberLeft + rememberRight) / 2];
    std::swap(nums[(rememberLeft + rememberRight) / 2], nums[rememberRight]);

    while (Ileft <= Iright) {
        while (Ileft <= Iright && nums[Ileft] < pivotElement) {
            ++Ileft;
        }
        while (Ileft <= Iright && nums[Iright] >= pivotElement) {
            --Iright;
        }

        if (Ileft < Iright) {
            std::swap(nums[Ileft], nums[Iright]);
        }
    }
    std::swap(nums[Ileft], nums[rememberRight]);
    QuickSort(nums, rememberLeft, Iright);
    QuickSort(nums, Ileft + 1, rememberRight);
}
