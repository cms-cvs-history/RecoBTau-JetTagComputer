import FWCore.ParameterSet.Config as cms

negativeCombinedMVABJetTags = cms.EDProducer("JetTagProducer",
	jetTagComputer = cms.string('negativeCombinedMVA'),
	tagInfos = cms.VInputTag(
		cms.InputTag("impactParameterTagInfos"),
		cms.InputTag("secondaryVertexTagInfos"),
		cms.InputTag("softMuonTagInfos"),
		cms.InputTag("softElectronTagInfos")
	)
)

negativeCombinedSecondaryVertexSoftPFLeptonV1BJetTags = cms.EDProducer("JetTagProducer",
	jetTagComputer = cms.string('negativeCombinedSecondaryVertexSoftPFLeptonV1'),
	tagInfos = cms.VInputTag(
		cms.InputTag("impactParameterTagInfos"),
		cms.InputTag("secondaryVertexTagInfos"),
		cms.InputTag("softPFMuonsTagInfos"),
		cms.InputTag("softPFElectronsTagInfos")
	)
)
