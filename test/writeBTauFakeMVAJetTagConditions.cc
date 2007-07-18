#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>

#include <boost/shared_ptr.hpp>

#include <TClass.h>
#include <TBufferXML.h>

#include "FWCore/Utilities/interface/Exception.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetupRecord.h"
#include "FWCore/Framework/interface/EventSetupRecordKey.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "CondFormats/PhysicsToolsObjects/interface/MVAComputer.h"
#include "CondFormats/DataRecord/interface/BTauGenericMVAJetTagComputerRcd.h"

using namespace PhysicsTools;

class writeBTauFakeMVAJetTagConditions : public edm::EDAnalyzer {
    public:
	explicit writeBTauFakeMVAJetTagConditions(
					const edm::ParameterSet &params);

	virtual void analyze(const edm::Event& iEvent,
	                     const edm::EventSetup& iSetup);

	virtual void endJob();

    private:
	std::string xmlCalibration;
};

writeBTauFakeMVAJetTagConditions::writeBTauFakeMVAJetTagConditions(
					const edm::ParameterSet &params) :
	xmlCalibration(params.getParameter<std::string>("xmlCalibration"))
{
}

void writeBTauFakeMVAJetTagConditions::analyze(const edm::Event& iEvent,
                                        const edm::EventSetup& iSetup)
{
	edm::ESHandle<Calibration::MVAComputerContainer> calib;
	iSetup.get<BTauGenericMVAJetTagComputerRcd>().get(calib);

	std::ofstream of(xmlCalibration.c_str());
	if (!of.good())
		throw cms::Exception("writeBTauFakeMVAJetTagConditions")
			<< "File \"" << xmlCalibration
			<< "\" could not be opened for writing." << std::endl;

	of << TBufferXML(TBuffer::kWrite).ConvertToXML(
		const_cast<void*>(static_cast<const void*>(calib.product())),
		TClass::GetClass("PhysicsTools::Calibration::MVAComputerContainer"),
		kTRUE, kFALSE);

	of.close();
}

void writeBTauFakeMVAJetTagConditions::endJob()
{
}

// define this as a plug-in
DEFINE_ANOTHER_FWK_MODULE(writeBTauFakeMVAJetTagConditions);
