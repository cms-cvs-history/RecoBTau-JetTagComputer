#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "RecoBTau/JetTagComputer/interface/JetTagComputerESProducer.h"
#include "RecoBTau/JetTagComputer/interface/GenericMVAJetTagComputer.h"

typedef JetTagComputerESProducer<GenericMVAJetTagComputer> GenericMVAJetTagESProducer;
DEFINE_FWK_EVENTSETUP_MODULE(GenericMVAJetTagESProducer);
