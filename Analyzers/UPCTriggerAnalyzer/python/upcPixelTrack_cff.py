import FWCore.ParameterSet.Config as cms

ESPPixelCPEGeneric = cms.ESProducer( "PixelCPEGenericESProducer",
    ComponentName = cms.string( "ESPPixelCPEGeneric" ),
    eff_charge_cut_lowX = cms.double( 0.0 ),
    eff_charge_cut_lowY = cms.double( 0.0 ),
    eff_charge_cut_highX = cms.double( 1.0 ),
    eff_charge_cut_highY = cms.double( 1.0 ),
    size_cutX = cms.double( 3.0 ),
    size_cutY = cms.double( 3.0 ),
    EdgeClusterErrorX = cms.double( 50.0 ),
    EdgeClusterErrorY = cms.double( 85.0 ),
    inflate_errors = cms.bool( False ),
    inflate_all_errors_no_trk_angle = cms.bool( False ),
    UseErrorsFromTemplates = cms.bool( True ),
    TruncatePixelCharge = cms.bool( True ),
    IrradiationBiasCorrection = cms.bool( False ),
    DoCosmics = cms.bool( False ),
    LoadTemplatesFromDB = cms.bool( True ),
    appendToDataLabel = cms.string( "" ),
    TanLorentzAnglePerTesla = cms.double( 0.106 ),
    PixelErrorParametrization = cms.string( "NOTcmsim" ),
    Alpha2Order = cms.bool( True ),
    ClusterProbComputationFlag = cms.int32( 0 )
)

ESPTTRHBuilderPixelOnly = cms.ESProducer( "TkTransientTrackingRecHitBuilderESProducer",
    ComponentName = cms.string( "ESPTTRHBuilderPixelOnly" ),
    StripCPE = cms.string( "Fake" ),
    PixelCPE = cms.string( "ESPPixelCPEGeneric" ),
    Matcher = cms.string( "StandardMatcher" ),
    ComputeCoarseLocalPositionFromDisk = cms.bool( False ),
    appendToDataLabel = cms.string( "" )
)

ESPHIPixelLayerTriplets = cms.ESProducer( "SeedingLayersESProducer",
    appendToDataLabel = cms.string( "" ),
    ComponentName = cms.string( "ESPHIPixelLayerTriplets" ),
    layerList = cms.vstring( 'BPix1+BPix2+BPix3',
      'BPix1+BPix2+FPix1_pos',
      'BPix1+BPix2+FPix1_neg',
      'BPix1+FPix1_pos+FPix2_pos',
      'BPix1+FPix1_neg+FPix2_neg' ),
    BPix = cms.PSet(
      hitErrorRZ = cms.double( 0.0060 ),
      hitErrorRPhi = cms.double( 0.0027 ),
      TTRHBuilder = cms.string( "ESPTTRHBuilderPixelOnly" ),
      HitProducer = cms.string( "hiSiPixelRecHits" ),
      useErrorsFromParam = cms.bool( True )
    ),
    FPix = cms.PSet(
      hitErrorRZ = cms.double( 0.0036 ),
      hitErrorRPhi = cms.double( 0.0051 ),
      TTRHBuilder = cms.string( "ESPTTRHBuilderPixelOnly" ),
      HitProducer = cms.string( "hiSiPixelRecHits" ),
      useErrorsFromParam = cms.bool( True )
    ),
    TEC = cms.PSet(  ),
    TID = cms.PSet(  ),
    TIB = cms.PSet(  ),
    TOB = cms.PSet(  )
)

hiSiPixelRecHits = cms.EDProducer( "SiPixelRecHitConverter",
    src = cms.InputTag( "siPixelClusters" ),
    CPE = cms.string( "ESPPixelCPEGeneric" )
)

hiPixelClusterVertices = cms.EDProducer( "HIPixelClusterVtxProducer",
    pixelRecHits = cms.string( "hiSiPixelRecHits" ),
    minZ = cms.double( -20.0 ),
    maxZ = cms.double( 20.05 ),
    zStep = cms.double( 0.1 )
)

hiPixelTracks = cms.EDProducer( "PixelTrackProducer",
    useFilterWithES = cms.bool( False ),
    RegionFactoryPSet = cms.PSet(
      ComponentName = cms.string( "HITrackingRegionForPrimaryVtxProducer" ),
      RegionPSet = cms.PSet(
	precise = cms.bool( True ),
	doVariablePtMin = cms.bool( True ),
	beamSpot = cms.InputTag( "offlineBeamSpot" ),
	useFixedError = cms.bool( True ),
	directionYCoord = cms.double( 1.0 ),
	sigmaZVertex = cms.double( 3.0 ),
	fixedError = cms.double( 2.0 ),
	directionXCoord = cms.double( 1.0 ),
	directionZCoord = cms.double( 0.0 ),
	VertexCollection = cms.InputTag( "hiPixelClusterVertices" ),
	ptMin = cms.double( 0.7 ),
	siPixelRecHits = cms.InputTag( "hiSiPixelRecHits" ),
	nSigmaZ = cms.double( 3.0 ),
	useFoundVertices = cms.bool( True ),
	originRadius = cms.double( 0.1 )
      )
    ),
    OrderedHitsFactoryPSet = cms.PSet(
      ComponentName = cms.string( "StandardHitTripletGenerator" ),
      GeneratorPSet = cms.PSet(
	useBending = cms.bool( True ),
	useFixedPreFiltering = cms.bool( False ),
	maxElement = cms.uint32( 100000 ),
	phiPreFiltering = cms.double( 0.3 ),
	extraHitRPhitolerance = cms.double( 0.06 ),
	useMultScattering = cms.bool( True ),
	ComponentName = cms.string( "PixelTripletHLTGenerator" ),
	extraHitRZtolerance = cms.double( 0.06 ),
	SeedComparitorPSet = cms.PSet(  ComponentName = cms.string( "none" ) )
      ),
      SeedingLayers = cms.string( "ESPHIPixelLayerTriplets" )
    ),
    FitterPSet = cms.PSet(
      ComponentName = cms.string( "PixelFitterByHelixProjections" ),
      TTRHBuilder = cms.string( "ESPTTRHBuilderPixelOnly" )
    ),
    FilterPSet = cms.PSet(
      doVariablePtMin = cms.bool( True ),
      beamSpot = cms.InputTag( "offlineBeamSpot" ),
      chi2 = cms.double( 1000.0 ),
      ComponentName = cms.string( "HIProtoTrackFilter" ),
      ptMin = cms.double( 1.0 ),
      siPixelRecHits = cms.InputTag( "hiSiPixelRecHits" ),
      tipMax = cms.double( 1.0 )
    ),
    CleanerPSet = cms.PSet(  ComponentName = cms.string( "PixelTrackCleanerBySharedHits" ) )
)	

siPixSeq = cms.Sequence(hiPixelClusterVertices+hiSiPixelRecHits+hiPixelTracks)
