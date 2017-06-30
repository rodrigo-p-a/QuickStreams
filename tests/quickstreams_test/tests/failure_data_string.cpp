#include "QuickStreamsTest.hpp"

// Verify the error received by the failure stream
// is the same as the error thrown in the failed stream
// given the error is of non-exception type: QString
void QuickStreamsTest::failure_data_string() {
	Trigger cpFirst;
	Trigger cpFailure;

	QVariant receivedError;

	auto firstStream = streams->create([&](
		const StreamHandle& stream, const QVariant& data
	) {
		Q_UNUSED(data)
		cpFirst.trigger();
		throw "something went wrong";
		stream.close();
	});

	auto failureStream = firstStream->failure([&](const QVariant& error) {
		receivedError = error;
		cpFailure.trigger();
		return QVariant();
	});

	Q_UNUSED(failureStream)

	QVERIFY(cpFirst.wait(100));
	QVERIFY(cpFailure.wait(100));

	// Ensure the error received by the failure stream is null
	// because QString is not a supported error type
	QCOMPARE(receivedError, QVariant());
}