/*
 * Overlaid header
 *
 * Copyright (C) 2013 Bastien Nocera <hadess@hadess.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "config.h"

#include "bacon-video-header-actor.h"

#define BACON_VIDEO_HEADER_ACTOR_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), BACON_TYPE_VIDEO_HEADER_ACTOR, BaconVideoHeaderActorPrivate))

struct BaconVideoHeaderActorPrivate
{
	GtkWidget *widget;
};

G_DEFINE_TYPE (BaconVideoHeaderActor, bacon_video_header_actor, GTK_CLUTTER_TYPE_ACTOR);

static void
bacon_video_header_actor_constructed (GObject *object)
{
	GdkRGBA transparent = { 0, 0, 0, 0 };
	BaconVideoHeaderActor *header;

	header = BACON_VIDEO_HEADER_ACTOR (object);

	g_object_set (object, "contents", header->priv->widget, "opacity", OVERLAY_OPACITY, "x-expand", TRUE, NULL);
	gtk_widget_show_all (header->priv->widget);

	/* Theming */
	gtk_style_context_add_class (gtk_widget_get_style_context (header->priv->widget), "osd");
	gtk_widget_override_background_color (gtk_clutter_actor_get_widget (GTK_CLUTTER_ACTOR (object)), 0, &transparent);
}

static void
bacon_video_header_actor_class_init (BaconVideoHeaderActorClass *klass)
{
        GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

        gobject_class->constructed = bacon_video_header_actor_constructed;

        g_type_class_add_private (klass, sizeof (BaconVideoHeaderActorPrivate));
}

static void
setup_object (BaconVideoHeaderActor *header,
	      const char            *name,
	      GtkWidget             *widget)
{
	/* Setup an easy way to lookup the widgets by name without
	 * exposing the API directly to totem or the plugin viewer */
	g_object_set_data (G_OBJECT (header), name, widget);
}

static GtkWidget *
add_button (BaconVideoHeaderActor *header,
	    GtkWidget             *button,
	    const char            *icon_name,
	    const char            *object_name)
{
	GtkWidget *image;

	image = gtk_image_new_from_icon_name (icon_name, GTK_ICON_SIZE_MENU);
	gtk_button_set_image (GTK_BUTTON (button), image);
	setup_object (header, object_name, button);

	gtk_header_bar_pack_end (GTK_HEADER_BAR (header->priv->widget), button);
	return button;
}

static void
bacon_video_header_actor_init (BaconVideoHeaderActor *header)
{
	GtkWidget *button;

	header->priv = BACON_VIDEO_HEADER_ACTOR_GET_PRIVATE (G_OBJECT (header));
	header->priv->widget = gtk_header_bar_new ();
	setup_object (header, "header", header->priv->widget);

	button = add_button (header, gtk_button_new (), "view-fullscreen-symbolic", "fullscreen_button");
	gtk_actionable_set_action_name (GTK_ACTIONABLE (button), "app.fullscreen");
	add_button (header, gtk_menu_button_new (), "emblem-system-symbolic", "button");
}

ClutterActor *
bacon_video_header_actor_new (void)
{
        return g_object_new (BACON_TYPE_VIDEO_HEADER_ACTOR, NULL);
}