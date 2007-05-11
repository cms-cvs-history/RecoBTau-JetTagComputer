#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "FWCore/Framework/interface/IOVSyncValue.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "CondFormats/PhysicsToolsObjects/interface/MVAComputer.h"

#include "PhysicsTools/MVAComputer/interface/BitSet.h"
#include "PhysicsTools/MVAComputer/interface/MVAComputer.h"

using namespace PhysicsTools::Calibration;

class testMakeMVAComputerCondDB : public edm::EDAnalyzer {
    public:
	explicit testMakeMVAComputerCondDB(const edm::ParameterSet &params);

	virtual void analyze(const edm::Event& iEvent,
	                     const edm::EventSetup& iSetup);

	virtual void endJob();

    private:
	std::string	record;
};

testMakeMVAComputerCondDB::testMakeMVAComputerCondDB(
					const edm::ParameterSet &params) :
	record(params.getUntrackedParameter<std::string>("record"))
{
}

void testMakeMVAComputerCondDB::analyze(const edm::Event& iEvent,
                                      const edm::EventSetup& iSetup)
{
}

void testMakeMVAComputerCondDB::endJob()
{
// set up some dummy calibration by hand for testing

	MVAComputerContainer *container = new MVAComputerContainer;
	MVAComputer *computer = &container->add("ImpactParameter");

// vars

	Variable var;
	var.name = "trackSip2d";
	computer->inputSet.push_back(var);

	var.name = "trackSip3d";
	computer->inputSet.push_back(var);

// normalize

	ProcNormalize *norm = new ProcNormalize();

	PhysicsTools::BitSet testSet(2);
	testSet[0] = testSet[1] = true;
	norm->inputVars = convert(testSet);

	PDF pdf;
	pdf.distr.push_back(0.8);
	pdf.distr.push_back(1.7);
	pdf.distr.push_back(1.4);
	pdf.distr.push_back(1.0);
	pdf.distr.push_back(0.8);
	pdf.range.first = -0.05;
	pdf.range.second = 0.2;
	norm->distr.push_back(pdf);
	norm->distr.push_back(pdf);

	computer->addProcessor(norm);

// likelihood

	ProcLikelihood *lkh = new ProcLikelihood();

	testSet = PhysicsTools::BitSet(4);
	testSet[2] = testSet[3] = true;
	lkh->inputVars = convert(testSet);

	pdf.distr.push_back(0.1);
	pdf.distr.push_back(0.5);
	pdf.distr.push_back(0.9);
	pdf.range.first = 0.0;
	pdf.range.second = 1.0;
	ProcLikelihood::SigBkg sigBkg;
	sigBkg.signal = pdf;
	pdf.distr.clear();
	pdf.distr.push_back(0.9);
	pdf.distr.push_back(0.5);
	pdf.distr.push_back(0.1);
	sigBkg.background = pdf;
	lkh->pdfs.push_back(sigBkg);
	lkh->pdfs.push_back(sigBkg);

	computer->addProcessor(lkh);

// optional

	ProcOptional *opt = new ProcOptional();

	testSet = PhysicsTools::BitSet(5);
	testSet[4] = true;
	opt->inputVars = convert(testSet);

	opt->neutralPos.push_back(-1.0);

	computer->addProcessor(opt);

// output

	computer->output = 5;

// test computer

	PhysicsTools::MVAComputer comp(computer);

	PhysicsTools::Variable::Value values[] = {
		PhysicsTools::Variable::Value("trackSip2d", 0.14),
		PhysicsTools::Variable::Value("trackSip3d", 0.12),
		PhysicsTools::Variable::Value("trackSip2d", 0.02),
		PhysicsTools::Variable::Value("trackSip3d", -0.03),
	};

	std::cout << comp.eval(values,
			       values + sizeof values / sizeof values[0])
		  << std::endl;

// write

	edm::Service<cond::service::PoolDBOutputService> dbService;
	if (!dbService.isAvailable()) {
		std::cerr << "DBService unavailable" << std::endl;
		return;
	}

	dbService->createNewIOV<MVAComputerContainer>(
		container, dbService->endOfTime(),
		"BTauGenericMVAJetTagComputerRcd");
}

// define this as a plug-in
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(testMakeMVAComputerCondDB);
