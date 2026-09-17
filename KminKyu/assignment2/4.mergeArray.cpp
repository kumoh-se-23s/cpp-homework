bool readAscSortedArray(int arr[], int size) {
    for (int arrSize = 0; arrSize < size; ++arrSize) {
        cin >> arr[arrSize];
    }
    for (int arrSize = 0; arrSize < size - 1; ++arrSize) {
        if (arr[arrSize] > arr[arrSize + 1]) {
            return false;
        }   
    }
    return true;
}

void mergeArray(int firstArray[], int secondArray[], int mergedArray[], int size) {
    int firstArrayIndex = 0, secondArrayIndex = 0, arrayIndex = 0;
    for (arrayIndex = 0; arrayIndex < size * 2 && firstArrayIndex < size && secondArrayIndex < size; ++arrayIndex) {
        if (firstArray[firstArrayIndex] <= secondArray[secondArrayIndex]) {
            mergedArray[arrayIndex] = firstArray[firstArrayIndex++];
        } else {
            mergedArray[arrayIndex] = secondArray[secondArrayIndex++];
        }
    }
    while (firstArrayIndex < size) {
        mergedArray[arrayIndex++] = firstArray[firstArrayIndex++];
    }
    while (secondArrayIndex < size) {
        mergedArray[arrayIndex++] = secondArray[secondArrayIndex++];
    }
}

void printArray(int array[], int size) {
    for (int index = 0; index < size; ++index) {
        if (index != 0) {
            cout << " ";
        }
        cout << array[index];
    }
    cout << endl;
}