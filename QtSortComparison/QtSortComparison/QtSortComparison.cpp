#include "QtSortComparison.h"
#include "_2p3qSeq.h"

QtSortComparison::QtSortComparison(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.ShowArray, SIGNAL(clicked()), this, SLOT(showArrayClicked()));
    connect(ui.ShowBubbleSortArray, SIGNAL(clicked()), this, SLOT(showBArrayClicked()));
    connect(ui.ShowShellSortArray, SIGNAL(clicked()), this, SLOT(showSArrayClicked()));
    connect(ui.ShowQuickSortArray, SIGNAL(clicked()), this, SLOT(showQArrayClicked()));
    connect(ui.StartAllSort, SIGNAL(clicked()), this, SLOT(startAllSort()));
    connect(ui.StartBubbleSort, SIGNAL(clicked()), this, SLOT(startBubbleSort()));
    connect(ui.StartShellSort, SIGNAL(clicked()), this, SLOT(startShellSort()));
    connect(ui.StartQuickSort, SIGNAL(clicked()), this, SLOT(startQuickSort()));
    connect(ui.GenerateArray, SIGNAL(clicked()), this, SLOT(generateClicked()));
    connect(ui.ArraySizeComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ArraySizeChanged(QString)));
    connect(this, SIGNAL(bProg(int)), this, SLOT(bubbleProgress(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(sProg(int)), this, SLOT(shellProgress(int)), Qt::QueuedConnection);
    connect(this, SIGNAL(qProg(int)), this, SLOT(quickProgress(int)), Qt::QueuedConnection);
    VectorSize = 10;
    bubbleTime = 0;
    shellTime = 0;
    quickTime = 0;
    generateIntVector(Vector, VectorSize);
    copyIntVector(Vector, bVector, VectorSize);
    copyIntVector(Vector, sVector, VectorSize);
    copyIntVector(Vector, qVector, VectorSize);
}

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}
void QtSortComparison::generateIntVector(std::vector<int>& vec, int size)
{
    vec.resize(size);
    std::srand(unsigned(std::time(nullptr)));
    generate(vec.begin(), vec.end(), std::rand);
}

void QtSortComparison::copyIntVector(std::vector<int>& vec1, std::vector<int>& vec2, int size)
{
    vec2.resize(size);
    for (int i = 0; i < size; vec2[i] = vec1[i], ++i);
}

/*  Exec and measure time 
    void (*pt2Func)(std::vector<int, std::allocator<int>>*, int, int), std::vector<int>* vec, int start, int end, double* outTime*/
void QtSortComparison::timeOf(Pt2Func pt2Func, QtSortComparison* member, std::vector<int>& vec, int start, int end, double* outTime)
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto t1 = high_resolution_clock::now();
    std::invoke(pt2Func, member, vec, start, end);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    *outTime = ms_double.count();
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int QtSortComparison::partition(std::vector<int>& vec, int low, int high)
{
    int pivot = vec[high];    // pivot
    int i = (low - 1);  // Index of smaller element

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (vec[j] <= pivot)
        {
            i++;    // increment index of smaller element
            std::swap(vec[i], vec[j]);
            qSwaps++;
        }
    }
    std::swap(vec[i + 1], vec[high]);
    qSwaps++;
    return (i + 1);
}

/* function to sort arr using bubbleSort */
void QtSortComparison::bubbleSort(std::vector<int>& vec, int _null, int n) {
    int swaps = 0;
    int _dif = n * n;
    emit bProg(0);
	for (int i = 0; i < n; i++) {
		bool noSwaps = true;
		for (int j = 0; j < n - i - 1; j++) {
			if (vec[j] > vec[j + 1]) {
                std::swap(vec[j], vec[j + 1]);
				noSwaps = false;
			}
		}
        emit bProg(swaps / (_dif/100));
		if (noSwaps)
			break;
	}
    emit bProg(100);
}

/* function to sort arr using shellSort */
void QtSortComparison::shellPrattSort(std::vector<int>& vec, int _null, int n)
{
    int swaps = 0;
    int _dif = std::pow(n, 1.25);
    emit sProg(0);
    // Start with a big gap, then reduce the gap
    for (int gap = n / 2; gap > 0; gap /= 2) {
        // Do a gapped insertion sort for this gap size.
        // The first gap elements arr[0..gap-1] are already in gapped order
        // keep adding one more element until the entire array is
        // gap sorted
        for (int i = gap; i < n; i += 1) {
            // add arr[i] to the elements that have been gap sorted
            // save arr[i] in temp and make a hole at position i
            int temp = vec[i];

            // shift earlier gap-sorted elements up until the correct
            // location for arr[i] is found
            int j;
            for (j = i; j >= gap && vec[j - gap] > temp; j -= gap)
                vec[j] = vec[j - gap];

            // put temp (the original arr[i]) in its correct location
            vec[j] = temp;
            swaps++;
        }
        emit sProg(swaps / (_dif / 100));
    }
    emit sProg(100);
}

/* function to sort arr using shellPrattSort */
void QtSortComparison::shellSort(std::vector<int>& vec, int _null, int n)
{
    int swaps = 0;
    int _dif = n*std::log10(n);
    emit sProg(0);
    _2p3qSeq seq;
    seq.generate(n);
    // Start with a big gap, then reduce the gap
    for (int gap = seq.popBack(); gap > 0; gap = seq.popBack()) {
        // Do a gapped insertion sort for this gap size.
        // The first gap elements arr[0..gap-1] are already in gapped order
        // keep adding one more element until the entire array is
        // gap sorted
        for (int i = gap; i < n; i += 1) {
            // add arr[i] to the elements that have been gap sorted
            // save arr[i] in temp and make a hole at position i
            int temp = vec[i];

            // shift earlier gap-sorted elements up until the correct
            // location for arr[i] is found
            int j;
            for (j = i; j >= gap && vec[j - gap] > temp; j -= gap)
                vec[j] = vec[j - gap];

            // put temp (the original arr[i]) in its correct location
            vec[j] = temp;
            swaps++;
        }
        emit sProg(swaps /(_dif / 100));
    }
    emit sProg(100);
}

//template< typename RandomAccessIterator, typename Compare >
//void shell_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
//{
//    for (auto d = (last - first) / 2; d != 0; d /= 2)
//        //нужен цикл для first = a[0..d-1]
//        for (auto i = first + d; i != last; ++i)
//            for (auto j = i; j - first >= d && comp(*j, *(j - d)); j -= d)
//                std::swap(*j, *(j - d));
//}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void QtSortComparison::quickSort(std::vector<int>& vec, int low, int high)
{
    int _dif = vec.size() * std::log10(vec.size());
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(vec, low, high);
        emit qProg(qSwaps / (_dif / 100) );
        // Separately sort elements before
        // partition and after partition
        quickSort(vec, low, pi - 1);
        quickSort(vec, pi + 1, high);
    }
}

void QtSortComparison::bubbleProgress(int x)
{
    ui.BubbleSortProgressBar->setValue(x);
}

void QtSortComparison::shellProgress(int x)
{
    ui.ShellSortProgressBar->setValue(x);
}

void QtSortComparison::quickProgress(int x)
{
    ui.QuickSortProgressBar->setValue(x);
}

void QtSortComparison::generateClicked()
{
    generateIntVector(Vector, VectorSize);
    copyIntVector(Vector, bVector, VectorSize);
    copyIntVector(Vector, sVector, VectorSize);
    copyIntVector(Vector, qVector, VectorSize);
}

void QtSortComparison::showArrayClicked()
{
    ui.ShowArray->setEnabled(false);
    for (auto i : Vector) {qInfo() << i;};
    //for (auto i = 0; i < sVector.size() - 1; qInfo() << sVector[i], ++i)
    ui.ShowArray->setEnabled(true);
}
void QtSortComparison::showBArrayClicked()
{
    ui.ShowArray->setEnabled(false);
    for (auto i : bVector) { qInfo() << i; };
    //for (auto i = 0; i < sVector.size() - 1; qInfo() << sVector[i], ++i)
    ui.ShowArray->setEnabled(true);
}

void QtSortComparison::showSArrayClicked()
{
    ui.ShowArray->setEnabled(false);
    for (auto i : sVector) { qInfo() << i; };
    //for (auto i = 0; i < sVector.size() - 1; qInfo() << sVector[i], ++i)
    ui.ShowArray->setEnabled(true);
}

void QtSortComparison::showQArrayClicked()
{
    ui.ShowArray->setEnabled(false);
    for (auto i : qVector) { qInfo() << i; };
    //for (auto i = 0; i < qVector.size() - 1; qInfo() << qVector[i], ++i)
    ui.ShowArray->setEnabled(true);
}

void QtSortComparison::startAllSort()
{
    std::thread tB(&QtSortComparison::StartBubbleSort, this);
    std::thread tS(&QtSortComparison::StartShellSort, this);
    qSwaps = 0;
    std::thread tQ(&QtSortComparison::StartQuickSort, this);
    tB.detach();
    tS.detach();
    tQ.detach();
}

void QtSortComparison::startBubbleSort()
{
    std::thread tB(&QtSortComparison::StartBubbleSort, this);
    tB.detach();
}

void QtSortComparison::startShellSort()
{
    std::thread tS(&QtSortComparison::StartShellSort, this);
    tS.detach();
}

void QtSortComparison::startQuickSort()
{
    qSwaps = 0;
    std::thread tQ(&QtSortComparison::StartQuickSort, this);
    tQ.detach();
}

void QtSortComparison::StartBubbleSort()
{
    timeOf(&QtSortComparison::bubbleSort, this,  bVector, 0, VectorSize, &bubbleTime);
    ui.BubbleSortTime->setText(QString(string_format("Time: \n%f ms.", bubbleTime).c_str()));
}

void QtSortComparison::StartShellSort()
{
    timeOf(&QtSortComparison::shellSort, this, sVector, 0, VectorSize, &shellTime);
    ui.ShellSortTime->setText(QString(string_format("Time: \n%f ms.", shellTime).c_str()));
}

void QtSortComparison::StartQuickSort()
{
    timeOf(&QtSortComparison::quickSort, this, qVector, 0, VectorSize, &quickTime);
    ui.QuickSortTime->setText(QString(string_format("Time: \n%f ms.", quickTime).c_str()));
}

void QtSortComparison::ArraySizeChanged(QString _item)
{
    VectorSize = _item.toInt();
}

QtSortComparison::~QtSortComparison()
{
}