#include <memory>
#include "gmock/gmock.h"
#include "presenter/ftpresenter.h"

using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;

namespace Mvp
{

namespace Test
{

class MockFTView : public Mvp::View::AbstractFTView
{
public:
    MOCK_METHOD0(initialize, void());
    MOCK_METHOD0(showView, void());
    MOCK_METHOD1(setFTTitle, void(const std::string&));
    MOCK_METHOD1(setFTStatus, void(FTStatus));
    MOCK_METHOD1(setFTError, void(FTError));
    MOCK_METHOD1(setSourceDesc, void(const std::string&));
    MOCK_METHOD1(setDestinationDesc, void(const std::string&));
    MOCK_METHOD1(setFileSize, void(const std::string&));
    MOCK_METHOD1(setFTSourceFileName, void(const std::string&));
    MOCK_METHOD1(setDirection, void(const FTDirection&));
    MOCK_METHOD0(destroyView, void());
    MOCK_METHOD2(updateTransferData, void(const std::string&, uint));

    MockFTView()
        : m_accept(true)
    {
        ON_CALL(*this, setFTStatus(_))
            .WillByDefault(Invoke(this, &MockFTView::acceptSetFTStatus));
    };

    void setDefaultAction(bool accept = true)
    {
        m_accept = accept;
    }

    void firePause(bool paused = true)
    {
        Poco::EventArgs args;

        if(paused)
            pause(this, args);
        else
            resume(this, args);
    }

    void fireCancel()
    {
        Poco::EventArgs args;

        cancel(this, args);
    }

private:
    void acceptSetFTStatus(FTStatus status)
    {
        Poco::EventArgs args;
        std::string fileName("/tmp/foo.txt");

        switch(status) {
            case AbstractFTView::Connecting:
                if (m_accept)
                    accepted(this, fileName);
                else
                    rejected(this, args);

                break;
        }
    }

    bool m_accept;
};

class MockFTService : public Mvp::Model::AbstractFTService
{
public:
    MOCK_METHOD0(partnerDisplayName, std::string());
    MOCK_METHOD0(fileSize, const std::string());
    MOCK_METHOD0(fileName, const std::string());
    MOCK_METHOD0(fileSource, const std::string());
    MOCK_METHOD0(fileDestination, const std::string());
    MOCK_METHOD0(status, Transfer::STATUS());
    MOCK_METHOD0(failureCode, Transfer::FAILUREREASON());
    MOCK_METHOD0(transferDirection, Transfer::TYPE());
    MOCK_METHOD1(acceptFT, void(const std::string&));
    MOCK_METHOD0(rejectFT, void());
    MOCK_METHOD0(cancelFT, void());
    MOCK_METHOD0(pauseFT, void());
    MOCK_METHOD0(resumeFT, void());
    MOCK_METHOD2(onStatusChanged, void(const void *,
                Transfer::STATUS&));
    MOCK_METHOD2(onProgressChanged, void(const void *,
                Poco::EventArgs&));

    MockFTService()
    {
        ON_CALL(*this, acceptFT(_))
            .WillByDefault(Invoke(this, &MockFTService::fakeAccept));

        ON_CALL(*this, rejectFT())
            .WillByDefault(Invoke(this, &MockFTService::fakeReject));

        ON_CALL(*this, cancelFT())
            .WillByDefault(Invoke(this, &MockFTService::fakeCancel));

        ON_CALL(*this, pauseFT())
            .WillByDefault(Invoke(this, &MockFTService::fakePause));

        ON_CALL(*this, resumeFT())
            .WillByDefault(Invoke(this, &MockFTService::fakeResume));
    };

private:
    void fakeAccept(const std::string&)
    {
        Transfer::STATUS status = Transfer::TRANSFERRING;

        onStatusChanged(this, status);
    }

    void fakeReject()
    {
        Transfer::STATUS status = Transfer::CANCELLED;

        onStatusChanged(this, status);
    }

    void fakeCancel()
    {
        Transfer::STATUS status = Transfer::CANCELLED;

        onStatusChanged(this, status);
    }

    void fakePause()
    {
        Transfer::STATUS status = Transfer::PAUSED;

        onStatusChanged(this, status);
    }

    void fakeResume()
    {
        Transfer::STATUS status = Transfer::TRANSFERRING;

        onStatusChanged(this, status);
    }
};

TEST(ftpresenter, test_acceptIncomingFT)
{
    Poco::SharedPtr<MockFTService> m(new MockFTService);

    Mvp::Presenter::FTPresenter presenter(m.get());
    Poco::SharedPtr<MockFTView> v(new MockFTView());
    EXPECT_CALL(*v, initialize());

    EXPECT_CALL(*m, transferDirection())
        .WillRepeatedly(Return(Transfer::INCOMING));
    EXPECT_CALL(*v, setDirection(_));
    EXPECT_CALL(*m, fileSource())
        .WillRepeatedly(Return(std::string("Foo:/someFooFile.txt")));
    EXPECT_CALL(*m, fileName());
    EXPECT_CALL(*v, setFTSourceFileName(_));
    EXPECT_CALL(*v, setSourceDesc(_));
    EXPECT_CALL(*m, fileSize())
        .WillRepeatedly(Return(std::string("1024")));
    EXPECT_CALL(*v, setFileSize(_));
    EXPECT_CALL(*m, partnerDisplayName());
    EXPECT_CALL(*v, setFTTitle(_));
    EXPECT_CALL(*m, status())
        .WillRepeatedly(Return(Transfer::NEW));
    EXPECT_CALL(*v, setFTStatus(Mvp::View::AbstractFTView::Connecting));


    // Accepting ft.
    EXPECT_CALL(*m, acceptFT(_));

    Transfer::STATUS status = Transfer::TRANSFERRING;
    EXPECT_CALL(*m, onStatusChanged(_, status));

    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();

    // Pausing ft.
    EXPECT_CALL(*m, pauseFT());

    status = Transfer::PAUSED;
    EXPECT_CALL(*m, onStatusChanged(_, status));

    v->firePause(true);

    // Resuming ft.
    EXPECT_CALL(*m, resumeFT());

    status = Transfer::TRANSFERRING;
    EXPECT_CALL(*m, onStatusChanged(_, status));

    v->firePause(false);
}

TEST(ftpresenter, test_rejectIncomingFT)
{
    Poco::SharedPtr<MockFTService> m(new MockFTService);

    Mvp::Presenter::FTPresenter presenter(m.get());
    Poco::SharedPtr<MockFTView> v(new MockFTView());
    v->setDefaultAction(false);
    EXPECT_CALL(*v, initialize());

    EXPECT_CALL(*m, transferDirection())
        .WillRepeatedly(Return(Transfer::INCOMING));
    EXPECT_CALL(*v, setDirection(_));
    EXPECT_CALL(*m, fileSource())
        .WillRepeatedly(Return(std::string("Foo:/someFooFile.txt")));
    EXPECT_CALL(*m, fileName());
    EXPECT_CALL(*v, setFTSourceFileName(_));
    EXPECT_CALL(*v, setSourceDesc(_));
    EXPECT_CALL(*m, fileSize())
        .WillRepeatedly(Return(std::string("1024")));
    EXPECT_CALL(*v, setFileSize(_));
    EXPECT_CALL(*m, partnerDisplayName());
    EXPECT_CALL(*v, setFTTitle(_));
    EXPECT_CALL(*m, status())
        .WillRepeatedly(Return(Transfer::NEW));
    EXPECT_CALL(*v, setFTStatus(Mvp::View::AbstractFTView::Connecting));


    // Rejecting ft.
    EXPECT_CALL(*m, rejectFT());

    Transfer::STATUS status = Transfer::CANCELLED;
    EXPECT_CALL(*m, onStatusChanged(_, status));

    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();
}

TEST(ftpresenter, test_outgoingFT)
{
    Poco::SharedPtr<MockFTService> m(new MockFTService);

    Mvp::Presenter::FTPresenter presenter(m.get());
    Poco::SharedPtr<MockFTView> v(new MockFTView());
    v->setDefaultAction(false);
    EXPECT_CALL(*v, initialize());

    EXPECT_CALL(*m, transferDirection())
        .WillRepeatedly(Return(Transfer::OUTGOING));
    EXPECT_CALL(*v, setDirection(_));
    EXPECT_CALL(*m, fileDestination())
        .WillRepeatedly(Return(std::string("Foo:/someFooFile.txt")));
    EXPECT_CALL(*v, setDestinationDesc(_));
    EXPECT_CALL(*m, fileSource())
        .WillRepeatedly(Return(std::string("/tmp/someFooFile.txt")));
    EXPECT_CALL(*v, setSourceDesc(_));
    EXPECT_CALL(*m, fileSize())
        .WillRepeatedly(Return(std::string("1024")));
    EXPECT_CALL(*v, setFileSize(_));
    EXPECT_CALL(*m, partnerDisplayName());
    EXPECT_CALL(*v, setFTTitle(_));
    EXPECT_CALL(*m, status())
        .WillRepeatedly(Return(Transfer::WAITING_FOR_ACCEPT));
    EXPECT_CALL(*v, setFTStatus(Mvp::View::AbstractFTView::WaitingForAccept));

    presenter.setView(v.get());

    EXPECT_CALL(*v, showView());
    presenter.showView();
}
};

};


int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}

