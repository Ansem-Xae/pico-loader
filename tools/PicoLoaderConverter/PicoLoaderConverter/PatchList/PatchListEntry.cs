namespace PicoLoaderConverter.PatchList;

sealed class PatchListEntry
{
    public string GameCode { get; init; } = string.Empty;
    public byte GameVersion { get; init; }
    public string GameName { get; init; } = string.Empty;
    public PatchListEntryPatch[] Patches { get; init; } = [];
}
