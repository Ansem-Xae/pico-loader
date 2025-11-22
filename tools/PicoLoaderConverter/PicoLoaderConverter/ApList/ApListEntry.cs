namespace PicoLoaderConverter.ApList;

sealed record ApListEntry(
    uint GameCode,
    byte GameVersion,
    DSProtectVersion DSProtectVersion,
    byte DSProtectFunctionMask,
    ushort RegularOverlayId,
    ushort SOverlayId,
    uint RegularOffset,
    uint SOffset)
{
    public const ushort OVERLAY_ID_STATIC_ARM9 = 0xFFFE;
    public const ushort OVERLAY_ID_INVALID = 0xFFFF;
}
