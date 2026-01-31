namespace PicoLoaderConverter.PatchList;

sealed class PatchListEntry
{
    public string GameCode { get; init; } = string.Empty;
    public byte GameVersion { get; init; }
    public PatchListEntryPatch[] Patches { get; init; } = [];
}
