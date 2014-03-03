#include <memory>
#include "gmock/gmock.h"
#include "model/historydata.h"
#include "presenter/historypresenter.h"

using ::testing::_;
using ::testing::Return;

// A vector to contain fake history data for testing purposes.
std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> > fakeHistory;

namespace Mvp
{

namespace Test
{

class MockHistoryView : public Mvp::View::AbstractHistoryView
{
public:
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD0(showView, void());
    MOCK_METHOD0(destroyView, void());
    MOCK_METHOD2(setHistory,
        void(std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> >, bool));

    MockHistoryView()
    {
    };

};

class MockHistoryService : public Mvp::Model::AbstractHistoryService
{
public:
    MOCK_METHOD1(getHistory,
        std::vector<Poco::SharedPtr<Mvp::Model::HistoryData> >(bool&));
    MOCK_METHOD0(markMessagesRead, void());

    MockHistoryService()
    {
        // Set the default return values for the service.
        ON_CALL(*this, getHistory(_)).WillByDefault(Return(fakeHistory));
    }

    void triggerHistoryChanged()
    {
        // Just emit the historyChanged signal.
        Poco::EventArgs args;
        historyChanged(this, args);
    }
};

TEST(historypresenter, test_historyChanged)
{
    Poco::SharedPtr<MockHistoryService> m(new MockHistoryService);

    Mvp::Presenter::HistoryPresenter presenter(m.get());
    Poco::SharedPtr<MockHistoryView> v(new MockHistoryView());
    EXPECT_CALL(*v, initialize());
    EXPECT_CALL(*m, getHistory(_));
    EXPECT_CALL(*v, setHistory(_, _));
    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();

    /* We expect getHistory and setHistory to be called when historyChanged
     * is triggered.
     */
    EXPECT_CALL(*m, getHistory(_));
    EXPECT_CALL(*v, setHistory(_, _));
    m->triggerHistoryChanged();
}

};

};

void initializeData()
{
    // Since we don't test any of the history item values, just use empty ones.
    Poco::SharedPtr<Mvp::Model::HistoryData>
        historyItem(new Mvp::Model::HistoryData);
    fakeHistory.push_back(historyItem);

    historyItem = new Mvp::Model::HistoryData;
    fakeHistory.push_back(historyItem);
}

int main(int argc, char** argv)
{
    // Create some fake history data.
    initializeData();

    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

